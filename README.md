# Proxy
a simple C++ proxy to forward queries

## Dependencies
### Boost.ASIO
#### 1- Using the official way
- Download and unzip : [https://dl.bintray.com/boostorg/release/1.75.0/source/boost_1_75_0.tar.gz](https://dl.bintray.com/boostorg/release/1.75.0/source/boost_1_75_0.tar.gz)

- build the `b2` tool. It is used to build other libs from Boost

    ```bash
    cd path/to/boost_1_75_0
    ./bootstrap.sh
    ```

- build libs needed. This will build the boost libs used by boost.asio .

    ```bash
    ./b2 --with-system --with-thread --with-date_time --with-regex --with-serialization stage
    ```

- install. It will install in the default location `/usr/local`.

    `find_package` in cmake will find `BoostConfig.cmake` and export the cmake targets. 

    ```bash
    ./b2 install
    ```
#### 2- Using HomeBrew (not tested)
This will install the latest version of Boost. Won't be able to install a specific version
```bash
brew install boost
```

## Test Commands
`telnet localhost 8080`

