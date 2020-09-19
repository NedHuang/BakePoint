# Java基础

# Callable Runnable, thread 需要补充

1. JDK 和 JRE 有什么区别？
    - JDK：Java Development Kit 的简称，java 开发工具包，提供了 java 的开发环境和运行环境。
    - JRE：Java Runtime Environment 的简称，java 运行环境，为 java 的运行提供了所需环境。
2. == 和 equals 的区别是什么？  
    - equals 是方法，而== 是操作符。基本类型只有 ==
    - ==
        - 基本类型：比较的是值是否相同
        - 引用类型：比较的是引用是否相同
    - .equals:
        默认情况下是引用比较，
    ```
        public boolean equals(Object obj) {
            return (this == obj);
            // 比较内存中的地址。
        }    
    ```
    - 很多类重新了 equals 方法，比如 String、Integer 等把它变成了值比较，所以一般情况下 equals 比较的是值是否相等.

3. 两个对象的 hashCode()相同，则 equals()也一定为 true吗？
    - 不对，两个对象的 hashCode()相同，equals()不一定 true。哈希值相等，键值不一定等。

4. final 在 java 中有什么作用？
    - final 修饰的类叫最终类，该类不能被继承。
    - final 修饰的方法不能被重写。
    - final 修饰的变量叫常量，常量必须初始化，初始化之后值就不能被修改。

5. java 中的 Math.round(-1.5) 等于多少？
    - 等于 -1， 以中间值(包含)向右取整

6. JAVA的基础类型是，string是基础类型吗？
    - 基础类型有 8 种：byte、boolean、char、short、int、float、long、double，。
    - String 属于对象，不属于基础类型。

7. java 中操作字符串都有哪些类？它们之间有什么区别？
    - String，StringBuffer，StringBuilder
    - String: 生成不可变对象。
    - StringBuffer、StringBuilder：可在原对象上进行操作。
    - StringBuffer:
        - 线程安全，多线程环境下推荐。
    - StringBuilder
        - 非线程安全，性能高，单线程环境下推荐。
    - 线程安全：指函数多线程环境中被调用时，能够正确地处理多个线程之间的共享变量，使程序功能正确完成。

8. String str="i"与 String str=new String("i")一样吗？
    - 不一样，因为内存的分配方式不一样。
    - String str="i"的方式，储存在常量池中。
    - String str=new String("i") 新建对象，会被分到堆内存中。

9. 如何将字符串反转？
    - 使用 StringBuilder 或者 stringBuffer 的 reverse() 方法。

10. String 类的常用方法都有那些？
    - indexOf()：返回指定字符的索引。
    - charAt()：返回指定索引处的字符。
    - replace()：字符串替换。
    - trim()：去除字符串两端空白。
    - split()：分割字符串，返回一个分割后的字符串数组。
    - getBytes()：返回字符串的 byte 类型数组。
    - length()：返回字符串长度。
    - toLowerCase()：将字符串转成小写字母。
    - toUpperCase()：将字符串转成大写字符。
    - substring()：截取字符串。
    - equals()：字符串比较。

11. 抽象类必须要有抽象方法吗？
    - 不需要，抽象类不一定非要有抽象方法。
```
    abstract class Cat {
        public static void sayHi() {
            System.out.println("hi~");
        }
    }
```
    - 抽象方法： 抽象类包含特别的成员方法，该方法的具体实现由它的子类确定，那么你可以在父类中声明该方法为抽象方法。
```
    public abstract class Employee
        {
        private String name;
        private String address;
        private int number;
        
        public abstract double computePay();        //抽象方法
        //其余代码
    }
```


12. 普通类和抽象类有哪些区别？
    - 普通类不能包含抽象方法，抽象类可以包含抽象方法。
    - 抽象类不能直接实例化，普通类可以直接实例化。需要被普通类继承

13. 抽象类能使用 final 修饰吗？
    - 不能，定义抽象类就是让其他类继承的，如果定义为 final 该类就不能被继承。

14. 接口和抽象类有什么区别？
    - 实现：抽象类的子类使用 extends 来继承；接口必须使用 implements 来实现接口。
    - 构造函数：抽象类可以有构造函数；接口不能有。
    - main 方法：抽象类可以有 main 方法，并且我们能运行它；接口不能有 main 方法。
    - 实现数量：类可以实现很多个接口；但是只能继承一个抽象类。
    - 访问修饰符：接口中的方法默认使用 public 修饰；抽象类中的方法可以是任意访问修饰符。

15. java 中 IO 流分为几种？
    - 按功能来分：输入流（input）、输出流（output）。
    - 按类型来分：字节流和字符流。
    - 字节流和字符流的区别是：字节流按 8 位传输以字节为单位输入输出数据，字符流按 16 位传输以字符为单位输入输出数据。

16. BIO、NIO、AIO 有什么区别
    - BIO：Block IO 同步阻塞式 IO，平常使用的传统 IO，它的特点是模式简单使用方便，并发处理能力低。
        - 服务端采用单线程，当accept一个请求后，在recv或send调用阻塞时，将无法accept其他请求（必须等上一个请求处recv或send完），无法处理并发
    - NIO：New IO 非同步阻塞 IO，是传统 IO 的升级，客户端和服务器端通过 Channel（通道）通讯，实现了多路复用。
        - 多路复用，又被称为 事件驱动： 实现一个线程可以监视多个文件句柄；一旦某个文件句柄就绪，就能够通知应用程序进行相应的读写操作。
    - AIO：Asynchronous IO 是 NIO 的升级，也叫 NIO2，实现了异步非堵塞 IO ，异步 IO 的操作基于事件和回调机制。
        - 回调: callback：
            - 类A的a()方法调用类B的b()方法
            - 类B的b()方法执行完毕主动调用类A的callback()方法
        -  回调分类：
            - 同步回调：调用完成后等待结果返回。
            - 异步回调:  为其开辟一条新的线程, 不等待返回结果

17. Files的常用方法都有哪些？
    - Files.exists()：检测文件路径是否存在。
    - Files.createFile()：创建文件。
    - Files.createDirectory()：创建文件夹。
    - Files.delete()：删除一个文件或目录。
    - Files.copy()：复制文件。
    - Files.move()：移动文件。
    - Files.size()：查看文件个数。
    - Files.read()：读取文件。
    - Files.write()：写入文件

18. java 静态方法和实例方法的区别
    - **静态**: 指在编译后所分配的内存会一直存在，直到程序退出内存才会释放这个空间
    - 被static修饰的成员变量和成员方法独立于该类的任何对象。也就是说，它不依赖类特定的实例，被类的所有实例共享。
    - 在外部调用静态方法时，可以使用"类名.方法名"的方式，也可以使用"对象名.方法名"的方式。而实例方法只有后面这种方式。也就是说，调用静态方法可以无需创建对象。
    - 静态方法在访问本类的成员时，只允许访问静态成员（即静态成员变量和静态方法），而不允许访问实例成员变量和实例方法；实例方法则无此限制。

# 容器， Container
- Java的容器是前人为我们设计好的一套存储对象和数据的一套轮子
1. Java 容器都有哪些？
    - Collection：主要是单个元素的集合，由List、Queue、Set三个接口区分不同的集合特征，然后由下面的具体的类来实现对应的功能。
        - vector不用了。。。
    - Map：有一组键值对的存储形式来保存，可以用键对象来查找值。
<img src="./images/Java/Container/Container.jpg" width="75%">

2.  List、Set、Queue、Map 之间的区别是什么.
    - List
        - 特点是所有的元素是可以重复的。
        - 分为ArrayList和LinkedList，前者底层是使用数组实现的List，后者是使用链表实现的List。
        - ArrayList:
            - ArrayList是一个可以动态增长的数组。(Java中数组的长度指定后不可改变)。
            - ArrayList由于底层是使用数组实现的，所以随机访问速度快，插入删除较慢
            - toArray：把LinkedList转化为Array.
        - LikedList:
            - LinkedList是使用链表实现的容器。
            - 在列表中插入和删除速度快，但是查找需要遍历整个链表，速度较慢。
    - Queue:
        - 队列是一个满足“先进先出”的数据结构。
        - LinkedList实现了Queue的接口，可以通过LinkedList向上转型为Queue。
        - PriorityQueue 优先级队列。出队顺序与元素的优先级有关。（写一个comparator)
``` 
    public class Main{
        Queue<User> q = new PriorityQueue<>(new UserComparator());
    }

    class UserComparator implements Comparator<User>{
        Public int compare(User u1, User u2){
           // code 
        }
    }
```

https://zhuanlan.zhihu.com/p/64147696

https://mp.weixin.qq.com/s?__biz=MzIwMTY0NDU3Nw==&mid=504458946&idx=1&sn=536fb5dfff90c8ef49f2f56dfba75aa8&chksm=0d0f018c3a78889a20056ef4c0a4c85fd22d149b3419ec4f63f843dd1b21e3ed322f2c3257b6#rd

https://zhuanlan.zhihu.com/p/29421226