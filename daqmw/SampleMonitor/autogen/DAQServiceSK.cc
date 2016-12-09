// This file is generated by omniidl (C++ backend)- omniORB_4_1. Do not edit.

#include "DAQService.hh"
#include <omniORB4/IOP_S.h>
#include <omniORB4/IOP_C.h>
#include <omniORB4/callDescriptor.h>
#include <omniORB4/callHandle.h>
#include <omniORB4/objTracker.h>


OMNI_USING_NAMESPACE(omni)

static const char* _0RL_library_version = omniORB_4_1;



void
NameValue::operator>>= (cdrStream &_n) const
{
  _n.marshalString(name,0);
  _n.marshalString(value,0);

}

void
NameValue::operator<<= (cdrStream &_n)
{
  name = _n.unmarshalString(0);
  value = _n.unmarshalString(0);

}

void
FatalErrorStatus::operator>>= (cdrStream &_n) const
{
  fatalTypes >>= _n;
  errorCode >>= _n;
  _n.marshalString(description,0);

}

void
FatalErrorStatus::operator<<= (cdrStream &_n)
{
  (::CORBA::Long&)fatalTypes <<= _n;
  (::CORBA::Long&)errorCode <<= _n;
  description = _n.unmarshalString(0);

}

void
Status::operator>>= (cdrStream &_n) const
{
  _n.marshalString(comp_name,0);
  state >>= _n;
  event_size >>= _n;
  comp_status >>= _n;

}

void
Status::operator<<= (cdrStream &_n)
{
  comp_name = _n.unmarshalString(0);
  (DAQLifeCycleState&)state <<= _n;
  (::CORBA::ULongLong&)event_size <<= _n;
  (CompStatus&)comp_status <<= _n;

}

DAQService_ptr DAQService_Helper::_nil() {
  return ::DAQService::_nil();
}

::CORBA::Boolean DAQService_Helper::is_nil(::DAQService_ptr p) {
  return ::CORBA::is_nil(p);

}

void DAQService_Helper::release(::DAQService_ptr p) {
  ::CORBA::release(p);
}

void DAQService_Helper::marshalObjRef(::DAQService_ptr obj, cdrStream& s) {
  ::DAQService::_marshalObjRef(obj, s);
}

DAQService_ptr DAQService_Helper::unmarshalObjRef(cdrStream& s) {
  return ::DAQService::_unmarshalObjRef(s);
}

void DAQService_Helper::duplicate(::DAQService_ptr obj) {
  if( obj && !obj->_NP_is_nil() )  omni::duplicateObjRef(obj);
}

DAQService_ptr
DAQService::_duplicate(::DAQService_ptr obj)
{
  if( obj && !obj->_NP_is_nil() )  omni::duplicateObjRef(obj);
  return obj;
}

DAQService_ptr
DAQService::_narrow(::CORBA::Object_ptr obj)
{
  if( !obj || obj->_NP_is_nil() || obj->_NP_is_pseudo() ) return _nil();
  _ptr_type e = (_ptr_type) obj->_PR_getobj()->_realNarrow(_PD_repoId);
  return e ? e : _nil();
}


DAQService_ptr
DAQService::_unchecked_narrow(::CORBA::Object_ptr obj)
{
  if( !obj || obj->_NP_is_nil() || obj->_NP_is_pseudo() ) return _nil();
  _ptr_type e = (_ptr_type) obj->_PR_getobj()->_uncheckedNarrow(_PD_repoId);
  return e ? e : _nil();
}

DAQService_ptr
DAQService::_nil()
{
#ifdef OMNI_UNLOADABLE_STUBS
  static _objref_DAQService _the_nil_obj;
  return &_the_nil_obj;
#else
  static _objref_DAQService* _the_nil_ptr = 0;
  if( !_the_nil_ptr ) {
    omni::nilRefLock().lock();
    if( !_the_nil_ptr ) {
      _the_nil_ptr = new _objref_DAQService;
      registerNilCorbaObject(_the_nil_ptr);
    }
    omni::nilRefLock().unlock();
  }
  return _the_nil_ptr;
#endif
}

const char* DAQService::_PD_repoId = "IDL:DAQService:1.0";


_objref_DAQService::~_objref_DAQService() {
  
}


_objref_DAQService::_objref_DAQService(omniIOR* ior, omniIdentity* id) :
   omniObjRef(::DAQService::_PD_repoId, ior, id, 1)
   
   
{
  _PR_setobj(this);
}

void*
_objref_DAQService::_ptrToObjRef(const char* id)
{
  if( id == ::DAQService::_PD_repoId )
    return (::DAQService_ptr) this;
  
  if( id == ::CORBA::Object::_PD_repoId )
    return (::CORBA::Object_ptr) this;

  if( omni::strMatch(id, ::DAQService::_PD_repoId) )
    return (::DAQService_ptr) this;
  
  if( omni::strMatch(id, ::CORBA::Object::_PD_repoId) )
    return (::CORBA::Object_ptr) this;

  return 0;
}

// Proxy call descriptor class. Mangled signature:
//  _cDAQLifeCycleState
class _0RL_cd_E6C5C158A9B04088_00000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_E6C5C158A9B04088_00000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, _user_exns, 0, upcall)
  {
    
  }
  
  
  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  
  static const char* const _user_exns[];

  DAQLifeCycleState result;
};

void _0RL_cd_E6C5C158A9B04088_00000000::marshalReturnedValues(cdrStream& _n)
{
  result >>= _n;

}

void _0RL_cd_E6C5C158A9B04088_00000000::unmarshalReturnedValues(cdrStream& _n)
{
  (DAQLifeCycleState&)result <<= _n;

}

const char* const _0RL_cd_E6C5C158A9B04088_00000000::_user_exns[] = {
  0
};

// Local call call-back function.
static void
_0RL_lcfn_E6C5C158A9B04088_10000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_E6C5C158A9B04088_00000000* tcd = (_0RL_cd_E6C5C158A9B04088_00000000*)cd;
  _impl_DAQService* impl = (_impl_DAQService*) svnt->_ptrToInterface(DAQService::_PD_repoId);
  tcd->result = impl->getState();


}

DAQLifeCycleState _objref_DAQService::getState()
{
  _0RL_cd_E6C5C158A9B04088_00000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_10000000, "getState", 9);


  _invoke(_call_desc);
  return _call_desc.result;


}
// Proxy call descriptor class. Mangled signature:
//  _cRTC_mReturnCode__t_i_cDAQCommand
class _0RL_cd_E6C5C158A9B04088_20000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_E6C5C158A9B04088_20000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, _user_exns, 0, upcall)
  {
    
  }
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  
  static const char* const _user_exns[];

  DAQCommand arg_0;
  RTC::ReturnCode_t result;
};

void _0RL_cd_E6C5C158A9B04088_20000000::marshalArguments(cdrStream& _n)
{
  arg_0 >>= _n;

}

void _0RL_cd_E6C5C158A9B04088_20000000::unmarshalArguments(cdrStream& _n)
{
  (DAQCommand&)arg_0 <<= _n;

}

void _0RL_cd_E6C5C158A9B04088_20000000::marshalReturnedValues(cdrStream& _n)
{
  result >>= _n;

}

void _0RL_cd_E6C5C158A9B04088_20000000::unmarshalReturnedValues(cdrStream& _n)
{
  (RTC::ReturnCode_t&)result <<= _n;

}

const char* const _0RL_cd_E6C5C158A9B04088_20000000::_user_exns[] = {
  0
};

// Local call call-back function.
static void
_0RL_lcfn_E6C5C158A9B04088_30000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_E6C5C158A9B04088_20000000* tcd = (_0RL_cd_E6C5C158A9B04088_20000000*)cd;
  _impl_DAQService* impl = (_impl_DAQService*) svnt->_ptrToInterface(DAQService::_PD_repoId);
  tcd->result = impl->setCommand(tcd->arg_0);


}

RTC::ReturnCode_t _objref_DAQService::setCommand(::DAQCommand command)
{
  _0RL_cd_E6C5C158A9B04088_20000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_30000000, "setCommand", 11);
  _call_desc.arg_0 = command;

  _invoke(_call_desc);
  return _call_desc.result;


}
// Proxy call descriptor class. Mangled signature:
//  _cDAQCommand
class _0RL_cd_E6C5C158A9B04088_40000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_E6C5C158A9B04088_40000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, _user_exns, 0, upcall)
  {
    
  }
  
  
  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  
  static const char* const _user_exns[];

  DAQCommand result;
};

void _0RL_cd_E6C5C158A9B04088_40000000::marshalReturnedValues(cdrStream& _n)
{
  result >>= _n;

}

void _0RL_cd_E6C5C158A9B04088_40000000::unmarshalReturnedValues(cdrStream& _n)
{
  (DAQCommand&)result <<= _n;

}

const char* const _0RL_cd_E6C5C158A9B04088_40000000::_user_exns[] = {
  0
};

// Local call call-back function.
static void
_0RL_lcfn_E6C5C158A9B04088_50000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_E6C5C158A9B04088_40000000* tcd = (_0RL_cd_E6C5C158A9B04088_40000000*)cd;
  _impl_DAQService* impl = (_impl_DAQService*) svnt->_ptrToInterface(DAQService::_PD_repoId);
  tcd->result = impl->getCommand();


}

DAQCommand _objref_DAQService::getCommand()
{
  _0RL_cd_E6C5C158A9B04088_40000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_50000000, "getCommand", 11);


  _invoke(_call_desc);
  return _call_desc.result;


}
// Proxy call descriptor class. Mangled signature:
//  _cDAQDone
class _0RL_cd_E6C5C158A9B04088_60000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_E6C5C158A9B04088_60000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, _user_exns, 0, upcall)
  {
    
  }
  
  
  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  
  static const char* const _user_exns[];

  DAQDone result;
};

void _0RL_cd_E6C5C158A9B04088_60000000::marshalReturnedValues(cdrStream& _n)
{
  result >>= _n;

}

void _0RL_cd_E6C5C158A9B04088_60000000::unmarshalReturnedValues(cdrStream& _n)
{
  (DAQDone&)result <<= _n;

}

const char* const _0RL_cd_E6C5C158A9B04088_60000000::_user_exns[] = {
  0
};

// Local call call-back function.
static void
_0RL_lcfn_E6C5C158A9B04088_70000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_E6C5C158A9B04088_60000000* tcd = (_0RL_cd_E6C5C158A9B04088_60000000*)cd;
  _impl_DAQService* impl = (_impl_DAQService*) svnt->_ptrToInterface(DAQService::_PD_repoId);
  tcd->result = impl->checkDone();


}

DAQDone _objref_DAQService::checkDone()
{
  _0RL_cd_E6C5C158A9B04088_60000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_70000000, "checkDone", 10);


  _invoke(_call_desc);
  return _call_desc.result;


}
// Proxy call descriptor class. Mangled signature:
//  void
class _0RL_cd_E6C5C158A9B04088_80000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_E6C5C158A9B04088_80000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, _user_exns, 0, upcall)
  {
    
  }
  
  
    
  
  static const char* const _user_exns[];

  
};

const char* const _0RL_cd_E6C5C158A9B04088_80000000::_user_exns[] = {
  0
};

// Local call call-back function.
static void
_0RL_lcfn_E6C5C158A9B04088_90000000(omniCallDescriptor*, omniServant* svnt)
{
  
  _impl_DAQService* impl = (_impl_DAQService*) svnt->_ptrToInterface(DAQService::_PD_repoId);
  impl->setDone();


}

void _objref_DAQService::setDone()
{
  _0RL_cd_E6C5C158A9B04088_80000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_90000000, "setDone", 8);


  _invoke(_call_desc);



}
// Proxy call descriptor class. Mangled signature:
//  void_i_cStatus
class _0RL_cd_E6C5C158A9B04088_a0000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_E6C5C158A9B04088_a0000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, _user_exns, 0, upcall)
  {
    
  }
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

    
  
  static const char* const _user_exns[];

  Status_var arg_0_;
  const Status* arg_0;
};

void _0RL_cd_E6C5C158A9B04088_a0000000::marshalArguments(cdrStream& _n)
{
  (const Status&) *arg_0 >>= _n;

}

void _0RL_cd_E6C5C158A9B04088_a0000000::unmarshalArguments(cdrStream& _n)
{
  arg_0_ = new Status;
  (Status&)arg_0_ <<= _n;
  arg_0 = &arg_0_.in();

}

const char* const _0RL_cd_E6C5C158A9B04088_a0000000::_user_exns[] = {
  0
};

// Local call call-back function.
static void
_0RL_lcfn_E6C5C158A9B04088_b0000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_E6C5C158A9B04088_a0000000* tcd = (_0RL_cd_E6C5C158A9B04088_a0000000*)cd;
  _impl_DAQService* impl = (_impl_DAQService*) svnt->_ptrToInterface(DAQService::_PD_repoId);
  impl->setStatus(*tcd->arg_0);


}

void _objref_DAQService::setStatus(const ::Status& stat)
{
  _0RL_cd_E6C5C158A9B04088_a0000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_b0000000, "setStatus", 10);
  _call_desc.arg_0 = &(::Status&) stat;

  _invoke(_call_desc);



}
// Proxy call descriptor class. Mangled signature:
//  _cStatus
class _0RL_cd_E6C5C158A9B04088_c0000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_E6C5C158A9B04088_c0000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, _user_exns, 0, upcall)
  {
    
  }
  
  
  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  
  static const char* const _user_exns[];

  Status_var result;
};

void _0RL_cd_E6C5C158A9B04088_c0000000::marshalReturnedValues(cdrStream& _n)
{
  (const Status&) result >>= _n;

}

void _0RL_cd_E6C5C158A9B04088_c0000000::unmarshalReturnedValues(cdrStream& _n)
{
  result = new Status;
  (Status&)result <<= _n;

}

const char* const _0RL_cd_E6C5C158A9B04088_c0000000::_user_exns[] = {
  0
};

// Local call call-back function.
static void
_0RL_lcfn_E6C5C158A9B04088_d0000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_E6C5C158A9B04088_c0000000* tcd = (_0RL_cd_E6C5C158A9B04088_c0000000*)cd;
  _impl_DAQService* impl = (_impl_DAQService*) svnt->_ptrToInterface(DAQService::_PD_repoId);
  tcd->result = impl->getStatus();


}

Status* _objref_DAQService::getStatus()
{
  _0RL_cd_E6C5C158A9B04088_c0000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_d0000000, "getStatus", 10);


  _invoke(_call_desc);
  return _call_desc.result._retn();


}
// Proxy call descriptor class. Mangled signature:
//  void_i_cNVList
class _0RL_cd_E6C5C158A9B04088_e0000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_E6C5C158A9B04088_e0000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, _user_exns, 0, upcall)
  {
    
  }
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

    
  
  static const char* const _user_exns[];

  NVList_var arg_0_;
  const NVList* arg_0;
};

void _0RL_cd_E6C5C158A9B04088_e0000000::marshalArguments(cdrStream& _n)
{
  (const NVList&) *arg_0 >>= _n;

}

void _0RL_cd_E6C5C158A9B04088_e0000000::unmarshalArguments(cdrStream& _n)
{
  arg_0_ = new NVList;
  (NVList&)arg_0_ <<= _n;
  arg_0 = &arg_0_.in();

}

const char* const _0RL_cd_E6C5C158A9B04088_e0000000::_user_exns[] = {
  0
};

// Local call call-back function.
static void
_0RL_lcfn_E6C5C158A9B04088_f0000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_E6C5C158A9B04088_e0000000* tcd = (_0RL_cd_E6C5C158A9B04088_e0000000*)cd;
  _impl_DAQService* impl = (_impl_DAQService*) svnt->_ptrToInterface(DAQService::_PD_repoId);
  impl->setCompParams(*tcd->arg_0);


}

void _objref_DAQService::setCompParams(const ::NVList& comp_params)
{
  _0RL_cd_E6C5C158A9B04088_e0000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_f0000000, "setCompParams", 14);
  _call_desc.arg_0 = &(::NVList&) comp_params;

  _invoke(_call_desc);



}
// Proxy call descriptor class. Mangled signature:
//  _cNVList
class _0RL_cd_E6C5C158A9B04088_01000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_E6C5C158A9B04088_01000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, _user_exns, 0, upcall)
  {
    
  }
  
  
  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  
  static const char* const _user_exns[];

  NVList_var result;
};

void _0RL_cd_E6C5C158A9B04088_01000000::marshalReturnedValues(cdrStream& _n)
{
  (const NVList&) result >>= _n;

}

void _0RL_cd_E6C5C158A9B04088_01000000::unmarshalReturnedValues(cdrStream& _n)
{
  result = new NVList;
  (NVList&)result <<= _n;

}

const char* const _0RL_cd_E6C5C158A9B04088_01000000::_user_exns[] = {
  0
};

// Local call call-back function.
static void
_0RL_lcfn_E6C5C158A9B04088_11000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_E6C5C158A9B04088_01000000* tcd = (_0RL_cd_E6C5C158A9B04088_01000000*)cd;
  _impl_DAQService* impl = (_impl_DAQService*) svnt->_ptrToInterface(DAQService::_PD_repoId);
  tcd->result = impl->getCompParams();


}

NVList* _objref_DAQService::getCompParams()
{
  _0RL_cd_E6C5C158A9B04088_01000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_11000000, "getCompParams", 14);


  _invoke(_call_desc);
  return _call_desc.result._retn();


}
// Proxy call descriptor class. Mangled signature:
//  void_i_clong
class _0RL_cd_E6C5C158A9B04088_21000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_E6C5C158A9B04088_21000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, _user_exns, 0, upcall)
  {
    
  }
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

    
  
  static const char* const _user_exns[];

  ::CORBA::Long arg_0;
};

void _0RL_cd_E6C5C158A9B04088_21000000::marshalArguments(cdrStream& _n)
{
  arg_0 >>= _n;

}

void _0RL_cd_E6C5C158A9B04088_21000000::unmarshalArguments(cdrStream& _n)
{
  (::CORBA::Long&)arg_0 <<= _n;

}

const char* const _0RL_cd_E6C5C158A9B04088_21000000::_user_exns[] = {
  0
};

// Local call call-back function.
static void
_0RL_lcfn_E6C5C158A9B04088_31000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_E6C5C158A9B04088_21000000* tcd = (_0RL_cd_E6C5C158A9B04088_21000000*)cd;
  _impl_DAQService* impl = (_impl_DAQService*) svnt->_ptrToInterface(DAQService::_PD_repoId);
  impl->setRunNo(tcd->arg_0);


}

void _objref_DAQService::setRunNo(::CORBA::Long run_no)
{
  _0RL_cd_E6C5C158A9B04088_21000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_31000000, "setRunNo", 9);
  _call_desc.arg_0 = run_no;

  _invoke(_call_desc);



}
// Proxy call descriptor class. Mangled signature:
//  _clong
class _0RL_cd_E6C5C158A9B04088_41000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_E6C5C158A9B04088_41000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, _user_exns, 0, upcall)
  {
    
  }
  
  
  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  
  static const char* const _user_exns[];

  ::CORBA::Long result;
};

void _0RL_cd_E6C5C158A9B04088_41000000::marshalReturnedValues(cdrStream& _n)
{
  result >>= _n;

}

void _0RL_cd_E6C5C158A9B04088_41000000::unmarshalReturnedValues(cdrStream& _n)
{
  (::CORBA::Long&)result <<= _n;

}

const char* const _0RL_cd_E6C5C158A9B04088_41000000::_user_exns[] = {
  0
};

// Local call call-back function.
static void
_0RL_lcfn_E6C5C158A9B04088_51000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_E6C5C158A9B04088_41000000* tcd = (_0RL_cd_E6C5C158A9B04088_41000000*)cd;
  _impl_DAQService* impl = (_impl_DAQService*) svnt->_ptrToInterface(DAQService::_PD_repoId);
  tcd->result = impl->getRunNo();


}

::CORBA::Long _objref_DAQService::getRunNo()
{
  _0RL_cd_E6C5C158A9B04088_41000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_51000000, "getRunNo", 9);


  _invoke(_call_desc);
  return _call_desc.result;


}
// Proxy call descriptor class. Mangled signature:
//  void_i_cFatalErrorStatus
class _0RL_cd_E6C5C158A9B04088_61000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_E6C5C158A9B04088_61000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, _user_exns, 0, upcall)
  {
    
  }
  
  void marshalArguments(cdrStream&);
  void unmarshalArguments(cdrStream&);

    
  
  static const char* const _user_exns[];

  FatalErrorStatus_var arg_0_;
  const FatalErrorStatus* arg_0;
};

void _0RL_cd_E6C5C158A9B04088_61000000::marshalArguments(cdrStream& _n)
{
  (const FatalErrorStatus&) *arg_0 >>= _n;

}

void _0RL_cd_E6C5C158A9B04088_61000000::unmarshalArguments(cdrStream& _n)
{
  arg_0_ = new FatalErrorStatus;
  (FatalErrorStatus&)arg_0_ <<= _n;
  arg_0 = &arg_0_.in();

}

const char* const _0RL_cd_E6C5C158A9B04088_61000000::_user_exns[] = {
  0
};

// Local call call-back function.
static void
_0RL_lcfn_E6C5C158A9B04088_71000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_E6C5C158A9B04088_61000000* tcd = (_0RL_cd_E6C5C158A9B04088_61000000*)cd;
  _impl_DAQService* impl = (_impl_DAQService*) svnt->_ptrToInterface(DAQService::_PD_repoId);
  impl->setFatalStatus(*tcd->arg_0);


}

void _objref_DAQService::setFatalStatus(const ::FatalErrorStatus& fatalStaus)
{
  _0RL_cd_E6C5C158A9B04088_61000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_71000000, "setFatalStatus", 15);
  _call_desc.arg_0 = &(::FatalErrorStatus&) fatalStaus;

  _invoke(_call_desc);



}
// Proxy call descriptor class. Mangled signature:
//  _cFatalErrorStatus
class _0RL_cd_E6C5C158A9B04088_81000000
  : public omniCallDescriptor
{
public:
  inline _0RL_cd_E6C5C158A9B04088_81000000(LocalCallFn lcfn,const char* op_,size_t oplen,_CORBA_Boolean upcall=0):
     omniCallDescriptor(lcfn, op_, oplen, 0, _user_exns, 0, upcall)
  {
    
  }
  
  
  void unmarshalReturnedValues(cdrStream&);
  void marshalReturnedValues(cdrStream&);
  
  
  static const char* const _user_exns[];

  FatalErrorStatus_var result;
};

void _0RL_cd_E6C5C158A9B04088_81000000::marshalReturnedValues(cdrStream& _n)
{
  (const FatalErrorStatus&) result >>= _n;

}

void _0RL_cd_E6C5C158A9B04088_81000000::unmarshalReturnedValues(cdrStream& _n)
{
  result = new FatalErrorStatus;
  (FatalErrorStatus&)result <<= _n;

}

const char* const _0RL_cd_E6C5C158A9B04088_81000000::_user_exns[] = {
  0
};

// Local call call-back function.
static void
_0RL_lcfn_E6C5C158A9B04088_91000000(omniCallDescriptor* cd, omniServant* svnt)
{
  _0RL_cd_E6C5C158A9B04088_81000000* tcd = (_0RL_cd_E6C5C158A9B04088_81000000*)cd;
  _impl_DAQService* impl = (_impl_DAQService*) svnt->_ptrToInterface(DAQService::_PD_repoId);
  tcd->result = impl->getFatalStatus();


}

FatalErrorStatus* _objref_DAQService::getFatalStatus()
{
  _0RL_cd_E6C5C158A9B04088_81000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_91000000, "getFatalStatus", 15);


  _invoke(_call_desc);
  return _call_desc.result._retn();


}
_pof_DAQService::~_pof_DAQService() {}


omniObjRef*
_pof_DAQService::newObjRef(omniIOR* ior, omniIdentity* id)
{
  return new ::_objref_DAQService(ior, id);
}


::CORBA::Boolean
_pof_DAQService::is_a(const char* id) const
{
  if( omni::ptrStrMatch(id, ::DAQService::_PD_repoId) )
    return 1;
  
  return 0;
}

const _pof_DAQService _the_pof_DAQService;

_impl_DAQService::~_impl_DAQService() {}


::CORBA::Boolean
_impl_DAQService::_dispatch(omniCallHandle& _handle)
{
  const char* op = _handle.operation_name();

  if( omni::strMatch(op, "getState") ) {

    _0RL_cd_E6C5C158A9B04088_00000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_10000000, "getState", 9, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "setCommand") ) {

    _0RL_cd_E6C5C158A9B04088_20000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_30000000, "setCommand", 11, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "getCommand") ) {

    _0RL_cd_E6C5C158A9B04088_40000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_50000000, "getCommand", 11, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "checkDone") ) {

    _0RL_cd_E6C5C158A9B04088_60000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_70000000, "checkDone", 10, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "setDone") ) {

    _0RL_cd_E6C5C158A9B04088_80000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_90000000, "setDone", 8, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "setStatus") ) {

    _0RL_cd_E6C5C158A9B04088_a0000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_b0000000, "setStatus", 10, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "getStatus") ) {

    _0RL_cd_E6C5C158A9B04088_c0000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_d0000000, "getStatus", 10, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "setCompParams") ) {

    _0RL_cd_E6C5C158A9B04088_e0000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_f0000000, "setCompParams", 14, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "getCompParams") ) {

    _0RL_cd_E6C5C158A9B04088_01000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_11000000, "getCompParams", 14, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "setRunNo") ) {

    _0RL_cd_E6C5C158A9B04088_21000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_31000000, "setRunNo", 9, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "getRunNo") ) {

    _0RL_cd_E6C5C158A9B04088_41000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_51000000, "getRunNo", 9, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "setFatalStatus") ) {

    _0RL_cd_E6C5C158A9B04088_61000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_71000000, "setFatalStatus", 15, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }

  if( omni::strMatch(op, "getFatalStatus") ) {

    _0RL_cd_E6C5C158A9B04088_81000000 _call_desc(_0RL_lcfn_E6C5C158A9B04088_91000000, "getFatalStatus", 15, 1);
    
    _handle.upcall(this,_call_desc);
    return 1;
  }


  return 0;
}

void*
_impl_DAQService::_ptrToInterface(const char* id)
{
  if( id == ::DAQService::_PD_repoId )
    return (::_impl_DAQService*) this;
  
  if( id == ::CORBA::Object::_PD_repoId )
    return (void*) 1;

  if( omni::strMatch(id, ::DAQService::_PD_repoId) )
    return (::_impl_DAQService*) this;
  
  if( omni::strMatch(id, ::CORBA::Object::_PD_repoId) )
    return (void*) 1;
  return 0;
}

const char*
_impl_DAQService::_mostDerivedRepoId()
{
  return ::DAQService::_PD_repoId;
}

POA_DAQService::~POA_DAQService() {}
