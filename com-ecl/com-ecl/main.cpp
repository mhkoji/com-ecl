#include "../com-ecl-proxy/com-ecl_h.h"
#include <iostream>

int main(void) {
  HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
  if (FAILED(hr)) {
    std::cerr << "CoInitialize failed";
    return 0;
  }

  IClassFactory *factory;
  hr = CoGetClassObject(CLSID_ComEcl, CLSCTX_LOCAL_SERVER, nullptr,
                        IID_IClassFactory, (LPVOID *)&factory);
  if (SUCCEEDED(hr)) {
    std::cerr << "CoGetClassObject" << std::endl;
    IAcc *acc;
    hr = factory->CreateInstance(nullptr, IID_IAcc, (void **)&acc);
    if (SUCCEEDED(hr)) {

      for (LONGLONG i = 0; i <= 20; i++) {
        acc->Inc(i);
        LONGLONG val;
        hr = acc->Value(&val);
        std::cerr << std::dec << "i= " << i << ", val=" << val
                  << ", hr=" << std::hex << (ULONG32)hr << std::endl;
        Sleep(30);
      }

    } else {
      std::cerr << "CreateInstance failed " << std::hex << (ULONG32)hr
                << std::endl;
    }

    factory->Release();
  } else {
    std::cerr << "CoGetClassObject failed " << std::hex << (ULONG32)hr
              << std::endl;
  }

  CoUninitialize();
  return 0;
}