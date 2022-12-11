# com-ecl
An example of COM out-of-process server with ECL

## Build com-ecl--all-systems.lib

- Install ecl into `C:/lisp/ecl`
- Run x64 Native Tools Command Prompt for VS 2022
- Run: `cd C:\lisp\ecl`
- Run `ecl --norc --eval "(require 'asdf)" --eval "(require 'cmp)" --load C:\lisp\com-ecl\lisp\com-ecl.asd`, and at the repl run `(asdf:make-build :com-ecl :type :static-library :monolithic t :move-here "./" :init-name "com_ecl_init")`
