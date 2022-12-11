#include "../com-ecl-proxy/com-ecl_h.h"
#include <iostream>
#include <string>

int main(void) {
  HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
  if (FAILED(hr)) {
    std::wcerr << "CoInitialize failed";
    return 0;
  }

  IClassFactory *factory;
  hr = CoGetClassObject(CLSID_ComEcl, CLSCTX_LOCAL_SERVER, nullptr,
                        IID_IClassFactory, (LPVOID *)&factory);
  if (SUCCEEDED(hr)) {
    IAcc *acc;
    hr = factory->CreateInstance(nullptr, IID_IAcc, (void **)&acc);
    if (SUCCEEDED(hr)) {

      for (LONGLONG i = 0; i <= 20; i++) {
        if ((i & 1) == 0) {
          acc->PutNum(i);
        } else {
          acc->PutStr(std::to_string(i).c_str());
        }

        LPWSTR val;
        hr = acc->ToString(&val);
        std::wcerr << std::dec << "i= " << i << ", val=" << val
                  << ", hr=" << std::hex << (ULONG32)hr << std::endl;
        if (SUCCEEDED(hr)) {
          CoTaskMemFree(val);
        }
        Sleep(30);
      }

      acc->Release();

    } else {
      std::wcerr << "CreateInstance failed " << std::hex << (ULONG32)hr
                << std::endl;
    }

    factory->Release();
  } else {
    std::wcerr << "CoGetClassObject failed " << std::hex << (ULONG32)hr
              << std::endl;
  }

  CoUninitialize();
  return 0;
}