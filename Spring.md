# Spring
https://wiki.jikexueyuan.com/project/spring/bean-definition.html
## 特性:
- 核心技术 ：依赖注入(DI)，AOP，事件(events)，资源，i18n，验证，数据绑定，类型转换，SpEL。
- 测试 ：模拟对象，TestContext框架，Spring MVC 测试，WebTestClient。
- 数据访问 ：事务，DAO支持，JDBC，ORM，编组XML。
- Web支持 : Spring MVC和Spring WebFlux Web框架。
- 集成 ：远程处理，JMS，JCA，JMX，电子邮件，任务，调度，缓存。
- 语言 ：Kotlin，Groovy，动态语言。
## 基本概念
- AOP：Aspect-oriented programming 面向切面编程：
    - 目的：Increase modularity by allowing the separation of cross-cutting concerns.
    - 方式：剖解开封装的对象内部，并将那些影响了多个类的公共行为封装到一个可重用模块，并将其名为“Aspect”，即切面。所谓“切面”，简单地说，就是将那些与业务无关，却为业务模块所共同调用的逻辑或责任封装起来，便于减少系统的重复代码，降低模块间的耦合度。
    - 切面（Aspect） 横切关注点的模块化（跨越应用程序多个模块的功能，比如 日志功能），这个关注点实现可能另外横切多个对象。
    - 连接点（Join point）： 连接点是在应用执行过程中能够插入切面的一个点。这个点可以是类的某个方法调用前、调用后、方法抛出异常后等。切面代码可以利用这些点插入到应用的正常流程之中，并添加行为。
    - 通知（Advice）： 在特定的连接点，AOP框架执行的动作。
        - 前置通知（Before）：在目标方法被调用之前调用通知功能。
        - 后置通知（After）：在目标方法完成之后调用通知，无论该方法是否发生异常。
        - 后置返回通知（After-returning）：在目标方法成功执行之后调用通知。
        - 后置异常通知（After-throwing）：在目标方法抛出异常后调用通知。
        - 环绕通知（Around）：通知包裹了被通知的方法，在被通知的方法调用之前和调用之后执行自定义的行为。

- IoC: Inversion of control 控制反转
    - IoC 是一种设计思想, Ioc意味着将你设计好的对象交给容器控制，而不是传统的在你的对象内部直接控制。
    - 通过控制反转，对象在被创建的时候，由一个调控系统内所有对象的外界实体，将其所依赖的对象的引用传递给它。也可以说，依赖被注入到对象中。
- DI:  Dependency Injection 依赖注入
    - 对对象的『依赖』是注入进来的，而和它的构造方式解耦了。构造它这个『控制』操作也交给了第三方，也就是控制反转。
    - a依赖b，但a不控制b的创建和销毁，仅使用b，那么b的控制权交给a之外处理，这叫控制反转（IOC），
    - a要依赖b，必然要使用b的instance，那么通过a的接口，把b传入；通过a的构造，把b传入；通过设置a的属性，把b传入；这个过程叫依赖注入（DI）。
```
        //例子 车子和引擎，创建一个引擎是200hp的车子的对象
        class Car{
            Engine e = new Engine() ;  
        }
        class Engine{
            Engine e = new Engine(200) ; //200马力
        }

        // IOC 的方式

        class Car{
            Engine e ;
            public Car(Engine e){
                this.e = e;
            };
        }
```

## Spring加载过程


## Bean 
- Bean是Spring框架在运行时管理的对象
- Bean 的管理包括：
    - 创建一个对象
    - 提供依赖项（例如其他bean，配置属性）
    - 拦截对象方法调用以提供额外的框架功能
    - 销毁一个对象

- 如何定义Spring bean
    - 使用构造型@Component注释（或其衍生物）注释你的类
    ```
        @Component
        class MySpringBeanClass {
            //...
        }
    ```
    在运行时，Spring会找到所有使用@Component或其派生类进行注释的类，并将它们用作bean定义。查找带注释的类的过程称为 **组件扫描**。
        - @Component衍生包括：@Service, @Repository, @Controller
    - 编写在自定义Java配置类中使用@Bean注释的bean工厂方法：
        - 工厂Bean: a bean that serves as a factory for creating other beans within the IoC container.

    - 在XML配置文件中声明bean定义


https://howtodoinjava.com/java-spring-framework-tutorials/



        <img src="./images/Spring/BeanLifeCycle.jpg" width="75%">