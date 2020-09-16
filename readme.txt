http://buildyourownlisp.com/contents

---

prerequisite:
    sudo pacman -S autoconf automake
    yay -S editline

cmake . -G "Ninja" && ninja && bin/Lispy

--- 

TODO:  
    Chapter 12 • Functions
    and later chapters