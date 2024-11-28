# QT

# Fundamentals:

## Object Model

The [QObject](https://doc.qt.io/qt-6/qobject.html) class forms the foundation of Qt's object model 
and is the parent class of many Qt classes. The object model introduces many mechanisms such as a 
meta-object system which allows run-time introspection, manipulation, and invocation of properties 
and methods in the object. It also serves as the basis for Qt's event system, which is a low-level 
way of communicating between [QObject](https://doc.qt.io/qt-6/qobject.html)-based objects. 
Another high-level form of communication is provided in Qt's signals and slots mechanism.

## Meta-Object System

The meta-object system is based on three things:

1. The [QObject](https://doc.qt.io/qt-6/qobject.html) class provides a base class for objects that 
can take advantage of the meta-object system.
2. The [Q_OBJECT](https://doc.qt.io/qt-6/qobject.html#Q_OBJECT) macro is used to enable meta-object 
features, such as dynamic properties, signals, and slots.

```
 Note. The Q_OBJECT macro starts a private section.
 To declare public members, use the 'public:' access modifier.
```

3. The [Meta-Object Compiler](https://doc.qt.io/qt-6/moc.html) `moc`) supplies 
each [QObject](https://doc.qt.io/qt-6/qobject.html) subclass with the necessary 
code to implement meta-object features.

The`moc` tool reads a C++ source file. If it finds one or more class declarations that contain 
the [Q_OBJECT](https://doc.qt.io/qt-6/qobject.html#Q_OBJECT) macro, it produces another C++ 
source file which contains the meta-object code for each of those classes. This generated source 
file is either `#include`'d into the class's source file or, more usually, compiled and 
linked with the class's implementation.

## Property System

To declare a property, use the [Q_PROPERTY](https://doc.qt.io/qt-6/qobject.html#Q_PROPERTY)() macro 
in a class that inherits [QObject](https://doc.qt.io/qt-6/qobject.html).

Here is an example showing how to export member variables as Qt properties using the `MEMBER` 
keyword. Note that a `NOTIFY` signal must be specified to allow QML property bindings.

```
  Q_PROPERTY(QColor color MEMBER m_color NOTIFY colorChanged)
    Q_PROPERTY(qreal spacing MEMBER m_spacing NOTIFY spacingChanged)
    Q_PROPERTY(QString text MEMBER m_text NOTIFY textChanged)
    ...
signals:
    void colorChanged();
    void spacingChanged();
    void textChanged(constQString &newText);

private:
QColor  m_color;
qreal   m_spacing;
QString m_text;
```

## [Signals and Slots](https://doc.qt.io/qt-6/signalsandslots.html)

In GUI programming, when we change one widget, we often want another widget to be notified. 
More generally, we want objects of any kind to be able to communicate with one another. 
For example, if a user clicks a **Close** button, we probably want the 
window's [close](https://doc.qt.io/qt-6/qwidget.html#close)() function to be called.

## [Thread Support](https://doc.qt.io/qt-6/threads.html)

Qt provides thread support in the form of platform-independent threading classes, a thread-safe 
way of posting events, and signal-slot connections across threads. This makes it easy to 
develop portable multithreaded Qt applications and take advantage of multiprocessor machines. 
Multithreaded programming is also a useful paradigm for performing time-consuming operations 
without freezing the user interface of an application.

TODO: RESEARCH and apply

# User Interfaces

The Qt framework's main user interface technologies are **Qt Quick** and **Qt Widgets**. Qt Quick 
interfaces are fluid, dynamic, and are best on touch interfaces. Qt Widgets are for creating 
complex desktop applications. You can create Qt Quick and Qt Widgets interfaces with the target 
platform's native look and feel.

# QML Applications

QML is a declarative language that allows user interfaces to be described in terms of their visual 
components and how they interact and relate with one another. It is a highly readable language 
that was designed to enable components to be interconnected in a dynamic manner, and it allows 
components to be easily reused and customized within a user interface. Using the `QtQuick` module, 
designers and developers can easily build fluid animated user interfaces in QML, and have the 
option of connecting these user interfaces to any back-end C++ libraries.

While the [Qt Qml](https://doc.qt.io/qt-6/qtqml-index.html) module provides the QML engine and 
language infrastructure, the Qt Quick module provides all the basic types necessary for creating 
user interfaces with QML. It provides a visual canvas and includes types for creating and animating 
visual components, receiving user input, creating data models and views and delayed object instantiation.

Despite all the benefits that QML and Qt Quick offer, they can be challenging in certain 
situations. [These](https://doc.qt.io/qt-6/qtquick-bestpractices.html) are some of the best practices to adopt:

- Bundle application resources
- Separate UI from business logic: We should strive to keep C++ types unaware of QML as much as 
possible. This can be achieved by "pushing" references to C++ types into QML.  
This can be done by using [required properties](https://doc.qt.io/qt-6/qtqml-syntax-objectattributes.html#required-properties) 
and setting them via [QQmlApplicationEngine::setInitialProperties](https://doc.qt.io/qt-6/qqmlapplicationengine.html#setInitialProperties). 
It is also possible to create one or multiple [singletons](https://doc.qt.io/qt-6/qqmlengine.html#QML_SINGLETON) 
- which will return all the data the C++ side wants to provide to QML.

## **Integrating QML and C++**

https://doc.qt.io/qt-6/qtqml-tutorials-extending-qml-example.html

https://doc.qt.io/qt-6/qtqml-cppintegration-overview.html#choosing-the-correct-integration-method-between-c-and-qml

https://doc.qt.io/qt-6/qtquickcontrols-chattutorial-example.html

QML is designed to be easily extensible through C++ code. The classes in the 
[Qt Qml](https://doc.qt.io/qt-6/qtqml-index.html) module enable QML objects to be loaded and 
manipulated from C++, and the nature of QML engine's integration with Qt's 
[meta object system](https://doc.qt.io/qt-6/metaobjects.html) enables C++ functionality to be 
invoked directly from QML. This allows the development of hybrid applications which are implemented 
with a mixture of QML, JavaScript and C++ code. It can be exposed to QML in a variety of 
ways [[ref](https://doc.qt.io/qt-6/qtqml-cppintegration-overview.html#choosing-the-correct-integration-method-between-c-and-qml)]:

- The class can be [registered as an instantiable QML type](https://doc.qt.io/qt-6/qtqml-cppintegration-definetypes.html#registering-an-instantiable-object-type), 
so that it can be instantiated and used like any ordinary [QML object type](https://doc.qt.io/qt-6/qtqml-typesystem-objecttypes.html) from QML code
- The class can be registered as a [Singleton Type](https://doc.qt.io/qt-6/qtqml-cppintegration-definetypes.html#registering-singleton-objects-with-a-singleton-type) so that a single instance of the class may be imported from QML code, allowing the instance's properties, methods and signals to be accessed from QML
- An instance of the class can be [embedded into QML code](https://doc.qt.io/qt-6/qtqml-cppintegration-contextproperties.html) 
as a *context property* or *context object*, allowing the instance's properties, methods and signals to be accessed from QML

### Register as an instantiable QML type:

To register a [QObject](https://doc.qt.io/qt-6/qobject.html)-derived class as an instantiable QML 
object type, add `QML_ELEMENT` or `QML_NAMED_ELEMENT(<name>)` to the class declaration.

Previously, C++ types were registered with qmlRegisterType(), qmlRegisterSingletonType(). This is 
not the best approach atm. QML_ELEMENT and QML_SINGLETON should be used instead. [[ref](https://www.qt.io/blog/qml-type-registration-in-qt-5.15)]

### Context Property:

When using `setContextProperty`, the property is available to every component loaded by the QML engine. 
Context properties are useful for objects that must be available as soon as the QML is loaded and 
cannot be instantiated in QML. (deprecated).  For small projects or projects where performance is 
not an issue, the context property is just fine. But for larger projects it should be avoided.

`setContextProperty` sets the object as value of a property in the very root node of your QML tree, 
so it basically looks like this:

```cpp
property var myContextProperty: MySetContextObject {}
ApplicationWindow {
    Item {
        Button {
        }
        ...
    }
    ...
}
```

The problem with context properties is that they "magically" inject state into your QML program. 
Your QML documents do not declare that they need this state, but they usually won't work without. 
Once the context properties are present, you can use them, but any tooling cannot properly track 
where they are added and where they are (or should be) removed. Context properties are invisible to 
QML tooling and the documents using them are impossible to validate statically.

Context properties always takes in a `QVariant`, which means that whenever you access the property 
it is re-evaluated because in between each access the property may be changed 
as `setContextProperty()` can be used at any moment in time.

Context properties are expensive to access, and hard to reason with. When you are writing QML code, 
you should strive to reduce the use of contextual variables (A variable that doesn't exist in the 
immediate scope, but the one above it.) and global state. Each QML document should be able to run 
with QML scene provided that the required properties are set. [[ref](https://github.com/Furkanzmc/QML-Coding-Guide?tab=readme-ov-file#ci-1-avoid-context-properties)]

It is better to set required properties via [QQmlApplicationEngine::setInitialProperties](https://doc.qt.io/qt-6/qqmlapplicationengine.html#setInitialProperties).

### **Registering Singleton type [**https://doc.qt.io/qt-6/qqmlengine.html#QML_SINGLETON**]**

There are bound to be cases where you have to provide a single instance for a functionality or 
common data access. In this situation, resort to using a singleton as it will have a better 
performance and be easier to read. Singletons are also a good option to expose enums to QML. 
[QML_SINGLETON](https://doc.qt.io/qt-6/qqmlengine.html#QML_SINGLETON) combined with 
[QML_ELEMENT](https://doc.qt.io/qt-6/qqmlengine.html#QML_ELEMENT) or 
[QML_NAMED_ELEMENT](https://doc.qt.io/qt-6/qqmlengine.html#QML_NAMED_ELEMENT) registers a singleton 
type that can be imported from QML.
