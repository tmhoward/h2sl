/****************************************************************************
** Meta object code from reading C++ file 'gui.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/util/h2sl/gui.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_h2sl__QGraphicsItem_Label[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_h2sl__QGraphicsItem_Label[] = {
    "h2sl::QGraphicsItem_Label\0"
};

void h2sl::QGraphicsItem_Label::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData h2sl::QGraphicsItem_Label::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject h2sl::QGraphicsItem_Label::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_h2sl__QGraphicsItem_Label,
      qt_meta_data_h2sl__QGraphicsItem_Label, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &h2sl::QGraphicsItem_Label::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *h2sl::QGraphicsItem_Label::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *h2sl::QGraphicsItem_Label::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_h2sl__QGraphicsItem_Label))
        return static_cast<void*>(const_cast< QGraphicsItem_Label*>(this));
    if (!strcmp(_clname, "QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< QGraphicsItem_Label*>(this));
    return QObject::qt_metacast(_clname);
}

int h2sl::QGraphicsItem_Label::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_h2sl__QGraphicsItem_Factor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      27,   53,   67,   67, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_h2sl__QGraphicsItem_Factor[] = {
    "h2sl::QGraphicsItem_Factor\0"
    "comment(std::string,bool)\0comment,error\0"
    "\0"
};

void h2sl::QGraphicsItem_Factor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QGraphicsItem_Factor *_t = static_cast<QGraphicsItem_Factor *>(_o);
        switch (_id) {
        case 0: _t->comment((*reinterpret_cast< const std::string(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData h2sl::QGraphicsItem_Factor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject h2sl::QGraphicsItem_Factor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_h2sl__QGraphicsItem_Factor,
      qt_meta_data_h2sl__QGraphicsItem_Factor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &h2sl::QGraphicsItem_Factor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *h2sl::QGraphicsItem_Factor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *h2sl::QGraphicsItem_Factor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_h2sl__QGraphicsItem_Factor))
        return static_cast<void*>(const_cast< QGraphicsItem_Factor*>(this));
    if (!strcmp(_clname, "QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< QGraphicsItem_Factor*>(this));
    return QObject::qt_metacast(_clname);
}

int h2sl::QGraphicsItem_Factor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void h2sl::QGraphicsItem_Factor::comment(const std::string & _t1, const bool & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_h2sl__QGraphicsItem_Grounding[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      30,   56,   70,   70, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_h2sl__QGraphicsItem_Grounding[] = {
    "h2sl::QGraphicsItem_Grounding\0"
    "comment(std::string,bool)\0comment,error\0"
    "\0"
};

void h2sl::QGraphicsItem_Grounding::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QGraphicsItem_Grounding *_t = static_cast<QGraphicsItem_Grounding *>(_o);
        switch (_id) {
        case 0: _t->comment((*reinterpret_cast< const std::string(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData h2sl::QGraphicsItem_Grounding::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject h2sl::QGraphicsItem_Grounding::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_h2sl__QGraphicsItem_Grounding,
      qt_meta_data_h2sl__QGraphicsItem_Grounding, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &h2sl::QGraphicsItem_Grounding::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *h2sl::QGraphicsItem_Grounding::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *h2sl::QGraphicsItem_Grounding::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_h2sl__QGraphicsItem_Grounding))
        return static_cast<void*>(const_cast< QGraphicsItem_Grounding*>(this));
    if (!strcmp(_clname, "QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< QGraphicsItem_Grounding*>(this));
    return QObject::qt_metacast(_clname);
}

int h2sl::QGraphicsItem_Grounding::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void h2sl::QGraphicsItem_Grounding::comment(const std::string & _t1, const bool & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_h2sl__GUI[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,   45,   59,   59, 0x09,
      60,   90,   59,   59, 0x29,
      98,   59,   59,   59, 0x09,
     135,  175,   59,   59, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_h2sl__GUI[] = {
    "h2sl::GUI\0_receive_comment(std::string,bool)\0"
    "comment,error\0\0_receive_comment(std::string)\0"
    "comment\0_line_edit_sentence_return_pressed()\0"
    "_combo_box_solutions_index_changed(int)\0"
    "index\0"
};

void h2sl::GUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GUI *_t = static_cast<GUI *>(_o);
        switch (_id) {
        case 0: _t->_receive_comment((*reinterpret_cast< const std::string(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2]))); break;
        case 1: _t->_receive_comment((*reinterpret_cast< const std::string(*)>(_a[1]))); break;
        case 2: _t->_line_edit_sentence_return_pressed(); break;
        case 3: _t->_combo_box_solutions_index_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData h2sl::GUI::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject h2sl::GUI::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_h2sl__GUI,
      qt_meta_data_h2sl__GUI, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &h2sl::GUI::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *h2sl::GUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *h2sl::GUI::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_h2sl__GUI))
        return static_cast<void*>(const_cast< GUI*>(this));
    return QWidget::qt_metacast(_clname);
}

int h2sl::GUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
