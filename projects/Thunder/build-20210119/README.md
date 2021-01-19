January 19, 2021 PC building scripts.
-

How to get them:
--

```
git clone git@github.com:Metrological/Documentation.git
cd Documentation
git checkout sander
cd projects/Thunder/build-20210119
```

How to get the sources (has to be done only once):
--

```
./fetchall.sh
```

How to build all packages:
--

```
./rebuild.sh
```

How to run `WPEFramework`:
--

```
./wpeframework-run.sh
```

How to open UI in your browser:
--

Check output of `./wpeframework-run.sh`, you will see a line like this:

```
[     113470 us] Accessor: http://192.168.2.21:8081/Service
```

This will tell you what `IP:port` to open in your browser (best to use Google Chrome in cognito mode). Don't add the `/Service` part, this will give you the services info in JSON format.

How to kill `WPEFramework`:
--

Press `q` followed by `ENTER` in the terminal running WPEFramework.

How to edit package and `make install` your changes:
--

1. Find the package you want to edit in the `repo` dir.
2. Make your changes.
3. Find your package in the `build` dir.
4. `cd` into its `build` dir.
5. Run `make -j4 install`.
6. Ask yourself if your change might influence other packages. Editing headers of `Thunder`, for example, might need a remake of `ThunderNanoServices`.
