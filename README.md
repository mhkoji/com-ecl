# com-ecl
An example of COM out-of-process server with ECL

For simplicity, we assume that ecl and the generated dll files are installed to `C:/lisp/ecl`

## Build

### Install ecl to `C:/lisp/ecl`

Ref: https://ecl.common-lisp.dev/static/manual/Building-ECL.html

- Run: `nmake ECL_CMP=1 ECL_ASDF=1 ECL_WIN64=1 GMP_TYPE=AMD64`
- Run: `nmake install prefix=C:\lisp\ecl`

### Build com-ecl--all-systems.lib

- Run x64 Native Tools Command Prompt for VS 2022
- Run: `cd C:\lisp\ecl`
- Run `ecl --norc --eval "(require 'asdf)" --eval "(require 'cmp)" --load <path-to-com-ecl-dir>\lisp\com-ecl.asd`, and at the repl run `(asdf:make-build :com-ecl :type :static-library :monolithic t :move-here "./" :init-name "com_ecl_init")`

### Build com-ecl-proxy, com-ecl-server and com-ecl

- Run Visual Studio
- Select `Release` configuration
- Build com-ecl-proxy,  com-ecl-server and com-ecl
- Run Powershell
- Run: `cp <path-to-com-ecl-dir>\com-ecl\x64\Release\com-ecl-proxy.dll C:\lisp\ecl\`
- Run: `cp <path-to-com-ecl-dir>\com-ecl\x64\Release\com-ecl-server.dll C:\lisp\ecl\`

### Register dll files

- Run cmd.exe as administrator
- Run: `regsvr32.exe C:\lisp\ecl\com-ecl-proxy.dll`
- Run: `regsvr32.exe C:\lisp\ecl\com-ecl-server.dll`

## Run com-ecl

Run `<path-to-com-ecl-dir>\com-ecl\x64\Release\com-ecl.exe` and the following messages are displayed:

```
i= 0, val=0, hr=0
i= 1, val=1, hr=0
i= 2, val=3, hr=0
i= 3, val=6, hr=0
i= 4, val=10, hr=0
i= 5, val=15, hr=0
i= 6, val=21, hr=0
i= 7, val=28, hr=0
i= 8, val=36, hr=0
i= 9, val=45, hr=0
i= 10, val=55, hr=0
i= 11, val=66, hr=0
i= 12, val=78, hr=0
i= 13, val=91, hr=0
i= 14, val=105, hr=0
i= 15, val=120, hr=0
i= 16, val=136, hr=0
i= 17, val=153, hr=0
i= 18, val=171, hr=0
i= 19, val=190, hr=0
i= 20, val=210, hr=0
```
