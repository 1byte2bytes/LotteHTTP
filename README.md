# LotteHTTP

## Building

```
mkdir build
cd build
cmake ..
make
```

You now have a `LotteHTTP` binary you can copy into a folder and run.

## Using LotteHTTP

Look in the Examples folder for example projects, the basic structure is like this

```
MySite
|_ LotteHTTP (web server binary)
|_ pages
   |_ index.lua
   |_ other_page.lua
```

A simple Lua file looks like this (the doPage function is the function called by LotteHTTP on a visit to the path):

```
function doPage()
    return "content to return to browser", "the-file/mimetype"
end
```

Files are called based on the URL browsed to, so http://localhost:3000/other_page will call `other_page.lua`. `index.lua` is a special file that gets called if no path is specified in the URL (aka a direct visit, http://localhost:3000/)