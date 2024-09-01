# A chisel3 AI acc 


# Build verilog

```bash
export PATH=/home/tannal/Downloads/sbt-1.10.1/sbt/bin:$PATH
sbt "runMain accelerator.Elaborate"
```

# Build simulator

```bash
cmake -G Ninja -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE='Debug'
ninja -C build
```