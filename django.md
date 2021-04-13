- MVC 模式
    - MVC 模式代表 Model-View-Controller（模型-视图-控制器） 模式。这种模式用于应用程序的分层开发。
        - Model（模型） - 模型代表一个存取数据的对象或 JAVA POJO。它也可以带有逻辑，在数据变化时更新控制器。
        - View（视图） - 视图代表模型包含的数据的可视化。
        - Controller（控制器） - 控制器作用于模型和视图上。它控制数据流向模型对象，并在数据变化时更新视图。它使视图与模型分离开。

- Django
    - MVT Model-View-Template
        - Django itself takes care of the Controller part.
            - leaving us with the template.

1. 什么是wsgi？
    - WSGI是Web Server Gateway Interface,
    - Python在处理HTTP请求时，规定的一种处理方式。如一个HTTP Request过来了，那么就有一个相应的处理函数来进行处理和返回结果。WSGI就是规定了处理函数的输入和输出格式。

2. django请求的生命周期？
    - 当用户在浏览器中输入url时,浏览器会生成请求头和请求体发给服务端请求头和请求体中会包含浏览器的动作(action),这个动作通常为get或者post,体现在url之中.. url经过Django中的wsgi,再经过Django的中间件,最后url到过路由映射表,在路由中一条一条进行匹配,
    - 一旦其中一条匹配成功就执行对应的视图函数,后面的路由就不再继续匹配了.
    - 视图函数根据客户端的请求查询相应的数据.返回给Django,然后Django把客户端想要的数据做为一个字符串返回给客户端.
    - 客户端浏览器接收到返回的数据,经过渲染后显示给用户.

3. 列举django的内置组件？
    - Admin是对model中对应的数据表进行增删改查提供的组件
    - model组件：负责操作数据库
    - form组件：1.生成HTML代码2.数据有效性校验3校验信息返回并展示
    - ModelForm组件即用于数据库操作,也可用于用户请求的验证

4. 列举django中间件的5个方法？以及django中间件的应用场景？
    - process_request : 请求进来时,权限认证
    - process_view : 路由匹配之后,能够得到视图函数
    - process_exception : 异常时执行
    - process_template_responseprocess : 模板渲染时执行
    - process_response : 请求有响应时执行
5. 简述什么是FBV和CBV？
    - FBV和CBV本质是一样的，基于函数的视图叫做FBV，基于类的视图叫做CBV
    - 在python中使用CBV的优点：
    - 提高了代码的复用性，可以使用面向对象的技术，比如Mixin（多继承）
    - 可以用不同的函数针对不同的HTTP方法处理，而不是通过很多if判断，提高代码可读性
    ```
        class XXX(view):
            def get(self, request):...

        utlpatterns=[
            path('a_url_path', views.XXX.as_view())
        ]
    ```

6. django的request对象是在什么时候创建的？
    - WSGIHandler类处理请求，把environ封装成 request
    ```
    class WSGIHandler(base.BaseHandler):
        request = self.request_class(environ)
    ```
7. 如何给CBV的程序添加装饰器？
    - 装饰器模式允许，动态地扩展额外的功能，而不需要改变原来的结构
    ```
            from django.utils.decorators import method_decorator
        1、给方法加：
            @method_decorator(check_login)
            def post(self, request):
                ...
        2、给dispatch加：
            @method_decorator(check_login)
            def dispatch(self, request, *args, **kwargs):
                ...
        3、给类加：
            @method_decorator(check_login, name="get")
            @method_decorator(check_login, name="post")
            class HomeView(View):
                ...

        @require_POST : 仅限POST方法的.. get请求报错 405

        @is_post_method 自定义装饰器：
        def is_post_method(func):
            def my_function(request, *args, **kwargs):
                if request.method == "POST":
                    # do somthing
                    return func(request, *args, **kwargs)
                else:
                    # do something
                    return HttpResponse(
                        json.dumps({
                            "result": False,
                            "data": [],
                            "message": u"请使用POST方法",
                            "code": -1
                        }), content_type='application/json')

            return my_function
    ```

8. 列举django orm 中所有的方法（QuerySet对象的所有方法）
    - all():                  查询所有结果 
    - filter(**kwargs):       它包含了与所给筛选条件相匹配的对象。获取不到返回None
    - get(**kwargs):          返回与所给筛选条件相匹配的对象，返回结果有且只有一个。
                              如果符合筛选条件的对象超过一个或者没有都会抛出错误。
    - exclude(**kwargs):      它包含了与所给筛选条件不匹配的对象
    - order_by(*field):       对查询结果排序
    - reverse():              对查询结果反向排序 
    - count():                返回数据库中匹配查询(QuerySet)的对象数量。 
    - first():                返回第一条记录 
    -  last():                返回最后一条记录 
    -  exists():              如果QuerySet包含数据，就返回True，否则返回False
    -  values(*field):        返回一个ValueQuerySet——一个特殊的QuerySet，运行后得到的
                              并不是一系 model的实例化对象，而是一个可迭代的字典序列
    - values_list(*field):   它与values()非常相似，它返回的是一个元组序列，values返回的是一个字典序列
    - distinct():            从返回结果中剔除重复纪录

9. select_related和prefetch_related的区别？
    - select_related通过多表join关联查询,一次性获得所有数据,只执行一次SQL查询
    - prefetch_related分别查询每个表,然后根据它们之间的关系进行处理,执行两次查询

10. filter和exclude的区别？
    - 两者取到的值都是QuerySet对象,filter选择满足条件的,exclude:排除满足条件的.

11. 列举django orm中三种能写sql语句的方法
    1. 使用execute执行自定义的SQL
        直接执行SQL语句（类似于pymysql的用法）
            # 更高灵活度的方式执行原生SQL语句
            from django.db import connection
            cursor = connection.cursor()
            cursor.execute("SELECT DATE_FORMAT(create_time, '%Y-%m') FROM blog_article;")
            ret = cursor.fetchall()
            print(ret)
    2. 使用extra方法 ：queryset.extra(select={"key": "原生的SQL语句"})
    3. 使用raw方法
        1. 执行原始sql并返回模型
        2. 依赖model多用于查询
    class中：
        queryset = xxx.objects.all()
        然后做处理
12. values和values_list的区别？
    - values : queryset类型的列表中是字典
    - values_list : queryset类型的列表中是元组

13. Cookie和session的区别
    - cookie:
        cookie是保存在浏览器端的键值对,可以用来做用户认证
    - session：
        将用户的会话信息保存在服务端,key值是随机产生的字符串,value值是session的内容
        依赖于cookie将每个用户的随机字符串保存到用户浏览器上
        Django中session默认保存在数据库中：django_session表
        flask,session默认将加密的数据写在用户的cookie中

14. 如何使用django orm批量创建数据？
    ```
        objs=[models.Book(title="图书{}".format(i+15)) for i in range(100)]
        models.Book.objects.bulk_create(objs)
    ```
16. django的Model中的ForeignKey字段中的on_delete参数有什么作用？
    - 删除关联表中的数据时,当前表与其关联的field的操作
        - CASCADE:这就是默认的选项，级联删除，你无需显性指定它。
        - PROTECT: 保护模式，如果采用该选项，删除的时候，会抛出ProtectedError错误。
        - SET_NULL: 置空模式，删除的时候，外键字段被设置为空，前提就是blank=True, null=True,定义该字段的时候，允许为空。
        - SET_DEFAULT: 置默认值，删除的时候，外键字段设置为默认值，所以定义外键的时候注意加上一个默认值。
        - SET(): 自定义一个值，该值当然只能是对应的实体了

17. django的模板中自定义filter和simple_tag的区别
    - 自定义filter：{{ 参数1|filter函数名:参数2 }}
    1. 可以与if标签来连用
    2. 自定义时需要写两个形参

18. Django中csrf的实现机制
    - django第一次响应来自某个客户端的请求时,后端随机产生一个token值，把这个token保存在SESSION状态中;同时,后端把这个token放到cookie中交给前端页面；
    - 下次前端需要发起请求（比如发帖）的时候把这个token值加入到请求数据或者头信息中,一起传给后端；Cookies:{csrftoken:xxxxx}
    - 后端校验前端请求带过来的token和SESSION里的token是否一致。

