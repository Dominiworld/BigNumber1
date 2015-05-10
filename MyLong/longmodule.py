# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.11
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_longmodule', [dirname(__file__)])
        except ImportError:
            import _longmodule
            return _longmodule
        if fp is not None:
            try:
                _mod = imp.load_module('_longmodule', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _longmodule = swig_import_helper()
    del swig_import_helper
else:
    import _longmodule
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


class LONG(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, LONG, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, LONG, name)
    __repr__ = _swig_repr
    def __init__(self): 
        this = _longmodule.new_LONG()
        try: self.this.append(this)
        except: self.this = this
    def __add__(self, *args): return _longmodule.LONG___add__(self, *args)
    def __sub__(self, *args): return _longmodule.LONG___sub__(self, *args)
    def __mul__(self, *args): return _longmodule.LONG___mul__(self, *args)
    def __div__(self, *args): return _longmodule.LONG___div__(self, *args)
    def __mod__(self, *args): return _longmodule.LONG___mod__(self, *args)
    def __gt__(self, *args): return _longmodule.LONG___gt__(self, *args)
    def __lt__(self, *args): return _longmodule.LONG___lt__(self, *args)
    def __eq__(self, *args): return _longmodule.LONG___eq__(self, *args)
    def __le__(self, *args): return _longmodule.LONG___le__(self, *args)
    def __ge__(self, *args): return _longmodule.LONG___ge__(self, *args)
    def PowMod(self, *args): return _longmodule.LONG_PowMod(self, *args)
    def FromTextFile(self, *args): return _longmodule.LONG_FromTextFile(self, *args)
    def FromBinFile(self, *args): return _longmodule.LONG_FromBinFile(self, *args)
    def ToBinFile(self, *args): return _longmodule.LONG_ToBinFile(self, *args)
    def ToTextFile(self, *args): return _longmodule.LONG_ToTextFile(self, *args)
    def Kill(self): return _longmodule.LONG_Kill(self)
    def IsEmpty(self): return _longmodule.LONG_IsEmpty(self)
    __swig_destroy__ = _longmodule.delete_LONG
    __del__ = lambda self : None;
LONG_swigregister = _longmodule.LONG_swigregister
LONG_swigregister(LONG)

# This file is compatible with both classic and new-style classes.


