# Spring
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