## Golang

1. new和make的区别
    - ew 的作用是初始化一个指向类型的指针(*T)
    - new函数是内建函数，函数定义：func new(Type) *Type
    - 使用new函数来分配空间。传递给new 函数的是一个类型，不是一个值。返回值是 指向这个新分配的零值的指针。
    ```go
        a := new(int)
    	fmt.Println(reflect.TypeOf(a))
        // output: *int
    ```
    - make 的作用是为 slice，map 或 chan 初始化并返回引用(T)。
    - make函数是内建函数，函数定义：func make(Type, size IntegerType) Type
    - 第一个参数是一个类型，第二个参数是长度，返回值是一个类型
    ```go
        b := make(map[string]int)
	    fmt.Println(b)
        // output: map[]
    ```

2. Printf()、Sprintf()、Fprintf()函数的区别用法是什么
    - Printf()，是把格式字符串输出到`标准输出`（一般是屏幕，可以重定向）。
    - Sprintf()，是把格式字符串输出到指定字符串中，第二个参数: char*。那就是目标字符串地址。 
    - Fprintf()，Fprintf()是格式化输出到一个stream，通常是到文件。第二个参数: FILE*。主要用于文件操作。
    ```go
    // printf
    printf("hello world");
    // hello world

    // Sprintf
    char buffer[50];
    int a = 10, b = 20, c;
    c = a + b;
    sprintf(buffer, "Sum of %d and %d is %d", a, b, c); // 
    printf("%s", buffer);
    // Sum of 10 and 20 is 30

    // Fprintf
    FILE *fptr = fopen("sample.txt", "w");
    ```
3. 数组和切片的区别
    - 数组 数组是具有固定长度且拥有零个或者多个相同数据类型元素的序列。 数组的长度是数组类型的一部分，所以[3]int 和 [4]int 是两种不同的数组类型。
    - 数组需要指定大小，或者使用(...)符号自动计算长度。 
    - 数组是值传递。
    ```go
    var array [10]int
    var array = [...]int{1,2,3,4,5}
    ```
    - 切片表示一个拥有相同类型元素的`可变长度`的序列。定义时不需要指定大小。
    - 它有三个属性：指针、长度和容量。
    - 切片是地址传递。
    - 切片可以通过数组来初始化，也可以通过内置函数make()初始化.初始化时len=cap,在append元素时如果容量cap不足时将按len的2倍扩容。 copy函数可以复制slice 
    ```go
    //定义
    var s1 = make([]byte, 5)
    var s2 = make([]byte, 5, 10)

    var a = []int{1, 2, 3, 4, 5, 6, 7, 8, 9}
    s1 := a[:4]
    s2 := a[3:7]
    fmt.Println(s1)     // [1 2 3 4]
    fmt.Println(s2)     // [4 5 6 7]
    s1[3] = 100
    fmt.Println(s1)     // [1 2 3 100]
    fmt.Println(s2)     // [100 5 6 7]
    ```

    - 切片像是一个结构体，包含了三个元素：
        1. 一个指向数组中切片指定的开始位置；
        2. 长度，即切片的长度，通过内置函数len获得；
        3. 最大长度，即切片的最大容量，通过内置函数cap获得。
        - 类似 redis中的 simple dynamic string（SDS）
        ```c
        struct sdshdr {
            // 记录buf数组中已使用字节的数量，即SDS所保存字符串的长度
            unsigned int len;
            // 记录buf数据中未使用的字节数量
            unsigned int free;
            // 字节数组，用于保存字符串
            char buf[];
        };
        ```

4. 一些命令
    - go env: #用于查看go的环境变量
    - go run: #用于编译并运行go源码文件
    - go build: #用于编译源码文件、代码包、依赖包
    - go get: #用于动态获取远程代码包
    - go install: #用于编译go文件，并将编译结构安装到bin、pkg目录
    - go clean: #用于清理工作目录，删除编译和安装遗留的目标文件
    - go version: #用于查看go的版本信息

5. go语言中的协程