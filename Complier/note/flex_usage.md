##### 编写flex 文件
语法见flex_bison_test文件夹中的代码
##### 编译flex 文件
````
flex flex.l
````

##### 编译 flex 生成的C代码
````
gcc lex.yy.c -o lexer -lfl   (注意链接flex库)
````