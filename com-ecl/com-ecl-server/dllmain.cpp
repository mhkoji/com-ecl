#include "../com-ecl-proxy/com-ecl_h.h"
#include "pch.h"
#include <OleCtl.h>
#include <shlwapi.h>
#include <string>

#include <ecl.h>
#include <mutex>

static LONG g_locks = 0;
static bool g_console_initialized = false;
static LONG g_inc_count = 0;

static HINSTANCE g_module_handle;

///

extern "C" {
void com_ecl_init(cl_object);
}

namespace ecl {

std::mutex g_init;
boolean g_initialized;

void Init() {
  std::lock_guard<std::mutex> lock(g_init);
  if (g_initialized) {
    return;
  }

  _wputenv_s(L"ECLDIR", L"C:\\lisp\\ecl\\");

  {
    char ecl_str[16];
    strncpy_s(ecl_str, "ecl", sizeof(ecl_str));
    char *argv[1] = {ecl_str};
    cl_boot(1, argv);
  }

  ecl_init_module(NULL, com_ecl_init);

  g_initialized = true;
}

void Destroy() { cl_shutdown(); }

void ToCString(cl_object obj, std::string *str) {
  struct ecl_string ecl_str = obj->string;
  for (int i = 0; i < ecl_str.fillp; i += 1) {
    *str += (decltype(ecl_str.elttype))ecl_str.self[i];
  }
}

class Acc {
public:
  Acc() { impl_ = cl_eval(c_string_to_object("(com-ecl:make-acc)")); }

  void PutNum(long long value) {
    cl_funcall(3, cl_eval(c_string_to_object("'com-ecl:acc-put")), impl_,
               ecl_make_fixnum(value));
  }

  void PutStr(const std::string &value) {
    cl_funcall(3, cl_eval(c_string_to_object("'com-ecl:acc-put")), impl_,
               ecl_make_constant_base_string(value.c_str(), -1));
  }

  void ToString(std::string *out) {
    cl_object str = cl_funcall(
        2, cl_eval(c_string_to_object("'com-ecl:acc-to-string")), impl_);
    ToCString(str, out);
  }

private:
  cl_object impl_;
};

} // namespace ecl

class Acc : public IAcc {
public:
  Acc() : count_(1) {
    ecl::Init();
    acc_ = new ecl::Acc();

    // Show console for debugging
    if (!g_console_initialized) {
      FILE *fp;
      AllocConsole();
      freopen_s(&fp, "CONOUT$", "w", stdout);
      freopen_s(&fp, "CONOUT$", "w", stderr);
      g_console_initialized = true;
    }
  }

  ~Acc() { delete acc_; }

  // IAcc
  virtual HRESULT __stdcall QueryInterface(REFIID riid,
                                           void **ppvObject) override {
    if (ppvObject == nullptr) {
      return E_INVALIDARG;
    }

    if (riid == IID_IUnknown || riid == IID_IAcc) {
      *ppvObject = static_cast<IAcc *>(this);
    } else {
      return E_NOINTERFACE;
    }
    AddRef();

    return S_OK;
  }
  virtual ULONG __stdcall AddRef(void) override {
    return InterlockedIncrement(&count_);
  }
  virtual ULONG __stdcall Release(void) override {
    ULONG count = InterlockedDecrement(&count_);
    if (count == 0) {
      delete this;
    }
    return count;
  }
  virtual HRESULT __stdcall PutNum(LONGLONG x) override {
    fprintf(stdout, "%s\n",
            (std::to_string(GetCurrentThreadId()) + ", " +
             std::to_string(++g_inc_count) + ", num=" + std::to_string(x))
                .c_str());
    acc_->PutNum(x);
    return S_OK;
  }
  virtual HRESULT __stdcall PutStr(LPCSTR x) override {
    fprintf(stdout, "%s\n",
            (std::to_string(GetCurrentThreadId()) + ", " +
             std::to_string(++g_inc_count) + ", str=" + x)
                .c_str());
    acc_->PutStr(x);
    return S_OK;
  }
  virtual HRESULT __stdcall ToString(LPWSTR *str) override {
    if (str == nullptr) {
      return E_INVALIDARG;
    }
    std::string out;
    acc_->ToString(&out);
    fprintf(stdout, "%s\n",
            (std::to_string(GetCurrentThreadId()) + ", " +
             std::to_string(++g_inc_count) + ", out=" + out)
                .c_str());
    return SHStrDupA(out.c_str(), str);
  }

private:
  ULONG count_;

  ecl::Acc *acc_;
};

class ComEclFactory : public IClassFactory {
public:
  ComEclFactory() : count_(1) {}
  // IClassFactory
  virtual HRESULT __stdcall QueryInterface(REFIID riid,
                                           void **ppvObject) override {
    if (ppvObject == nullptr) {
      return E_INVALIDARG;
    }

    if (riid == IID_IUnknown || riid == IID_IClassFactory) {
      *ppvObject = static_cast<IClassFactory *>(this);
    } else {
      return E_NOINTERFACE;
    }
    AddRef();

    return S_OK;
  }
  virtual ULONG __stdcall AddRef(void) override {
    LockModule();
    return InterlockedIncrement(&count_);
  }
  virtual ULONG __stdcall Release(void) override {
    ULONG count = InterlockedDecrement(&count_);
    if (count == 0) {
      UnlockModule();
      delete this;
    }
    return count;
  }
  virtual HRESULT __stdcall CreateInstance(IUnknown *pUnkOuter, REFIID riid,
                                           void **ppvObject) override {
    if (ppvObject == nullptr) {
      return E_INVALIDARG;
    }
    if (pUnkOuter != nullptr) {
      return CLASS_E_NOAGGREGATION;
    }

    *ppvObject = nullptr;

    IAcc *acc = new Acc();
    if (acc == nullptr) {
      return E_OUTOFMEMORY;
    }

    HRESULT result = acc->QueryInterface(riid, ppvObject);
    acc->Release();

    return result;
  }
  virtual HRESULT __stdcall LockServer(BOOL fLock) override {
    if (fLock) {
      LockModule();
    } else {
      UnlockModule();
    }
    return S_OK;
  }

private:
  ULONG count_;

  void LockModule() { InterlockedIncrement(&g_locks); }

  void UnlockModule() { InterlockedDecrement(&g_locks); }
};

///

// UUIDs are copied from com-ecl.idl
static const char *g_entries[][3] = {
    {"CLSID\\{4e6665df-5646-4815-b0e9-81d6af1c51eb}", 0, "com-ecl"},
    {"CLSID\\{4e6665df-5646-4815-b0e9-81d6af1c51eb}", "AppID",
     "{d735491b-36f3-4739-bd3a-a60bb9a96b3f}"},
    {"CLSID\\{4e6665df-5646-4815-b0e9-81d6af1c51eb}\\InprocServer32", 0,
     "<filename>"},
    {"CLSID\\{4e6665df-5646-4815-b0e9-81d6af1c51eb}\\InprocServer32",
     "ThreadingModel", "Apartment"},
    {"AppID\\{d735491b-36f3-4739-bd3a-a60bb9a96b3f}", "DllSurrogate", ""},
};

STDAPI DllUnregisterServer(void) {
  HRESULT hr = S_OK;
  int nEntries = sizeof(g_entries) / sizeof(*g_entries);
  for (int i = nEntries - 1; i >= 0; i--) {
    long err = RegDeleteKeyA(HKEY_CLASSES_ROOT, g_entries[i][0]);
    if (err != ERROR_SUCCESS) {
      hr = S_FALSE;
    }
  }

  return hr;
}

STDAPI DllRegisterServer(void) {
  char modue_file_name[MAX_PATH];
  GetModuleFileNameA(g_module_handle, modue_file_name, MAX_PATH);

  int entry_count = sizeof(g_entries) / sizeof(*g_entries);
  for (int i = 0; i < entry_count; i++) {
    const char *key_name = g_entries[i][0];
    const char *value_name = g_entries[i][1];
    std::string value = g_entries[i][2];

    if (value == "<filename>") {
      value = modue_file_name;
    }

    HKEY hkey;
    long err = RegCreateKeyA(HKEY_CLASSES_ROOT, key_name, &hkey);
    if (ERROR_SUCCESS == err) {
      err = RegSetValueExA(hkey, value_name, 0, REG_SZ,
                           (const BYTE *)value.c_str(),
                           static_cast<DWORD>(value.size() + 1));
      RegCloseKey(hkey);
    } else if (ERROR_SUCCESS != err) {
      DllUnregisterServer();
      return SELFREG_E_CLASS;
    }
  }
  return S_OK;
}

STDAPI DllCanUnloadNow(void) { return 0 == g_locks ? S_OK : S_FALSE; }

STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid,
                         _Outptr_ LPVOID FAR *ppv) {
  if (rclsid != CLSID_ComEcl) {
    return CLASS_E_CLASSNOTAVAILABLE;
  }

  ComEclFactory *factory = new ComEclFactory();
  if (!factory) {
    return E_OUTOFMEMORY;
  }
  HRESULT hr = factory->QueryInterface(riid, ppv);
  factory->Release();
  return hr;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:
    g_module_handle = hModule;
    break;
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
    break;
  case DLL_PROCESS_DETACH:
    ecl::Destroy();
    break;
  }
  return TRUE;
}
