# upp

Upp is a lightweight single page web application where you can enable upload areas to your server.
You can create upload areas available for everyone to use, or protect with a passphrase.


## Installing

Download the latest version from this repository's [release](https://github.com/fele-systems/upp/releases) page and extract it.

```shell
wget -nv https://github.com/fele-systems/upp/releases/download/v1.0-alpha/upp.zip
unzip upp.zip
cd upp
chmod +x upp
```

## Building from source

First, make sure you have conan and CMake installed.

```shell
wget -O conan-ubuntu-64.deb https://github.com/conan-io/conan/releases/download/2.4.1/conan-2.4.1-amd64.deb
dpkg -i conan-ubuntu-64.deb
rm conan-ubuntu-64.deb
```

If you haven't created your conan profile, execute `conan profile detect` before these steps.

```shell
conan install . -of build --build=missing -s build_type=Release
cmake -B ./build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build ./build --config Release
```

Then run it with `build/upp`.

## Getting started

Make sure you've a file named upp.yaml in the same directory as the executable (and the current working directory). This file will tell upp where it should store the uploaded files.

Sample configuration:

```yaml
server:
  base_path: ./
  port: 80
  dirs:
    - path: /tmp
      passphrase:
    - path: uploads/uploaded.txt
      passphrase: uploadin
      file: true
```

The upload areas can be a directory, which several files can be stored, or can be a file (`file: true`), in which only a sigle file can exist and it will always be overrided when the next upload comes.

The `base_path` is the parent path of any relative path'ed upload areas (like the uploads.uploaded.txt upload area above).

The `passphrase` is a password that is required in order to upload files.