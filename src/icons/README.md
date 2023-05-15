# Converting svg to png

On MacOS:

~~~
    for i in *; do rsvg-convert -d 1200 -p 1200 -w 512 -h 512 $i -o `echo $i | sed -e 's/svg$/png/'`; done
~~~
