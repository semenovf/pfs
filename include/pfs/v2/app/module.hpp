#ifndef __PFS_V2_MODULE_HPP__
#define __PFS_V2_MODULE_HPP__

#include <pfs/cxxlang.hpp>

#include "modulus.hpp"

#define PFS_V2_DETECTOR_CAST(slot) reinterpret_cast<detector_handler>(& slot)
#define PFS_V2_EMITTER_CAST(e)     reinterpret_cast<void *>(& e)
#define PFS_V2_MODULE_API extern "C" PFS_DLL_API

#define PFS_V2_MODULE_EMITTER(id, em) { id , PFS_V2_EMITTER_CAST(em) }
#define PFS_V2_MODULE_DETECTOR(id, dt) { id , PFS_V2_DETECTOR_CAST(dt) }

#define PFS_V2_MODULE_EMITTERS_EXTERN                                          \
emitter_mapper_pair const *                                                    \
get_emitters (int & count) pfs_override;

#define PFS_V2_MODULE_EMITTERS_BEGIN(XMOD)                                     \
XMOD::emitter_mapper_pair const *                                              \
XMOD::get_emitters (int & count)                                               \
{                                                                              \
	static emitter_mapper_pair __emitter_mapper[] = {

#define PFS_V2_MODULE_EMITTERS_INLINE_BEGIN                                    \
emitter_mapper_pair const *                                                    \
get_emitters (int & count) pfs_override                                        \
{                                                                              \
	static emitter_mapper_pair __emitter_mapper[] = {

#define PFS_V2_MODULE_EMITTERS_END                                             \
	};                                                                         \
	count = sizeof(__emitter_mapper)/sizeof(__emitter_mapper[0]);              \
	return & __emitter_mapper[0];                                              \
}

#define PFS_V2_MODULE_DETECTORS_EXTERN                                         \
detector_mapper_pair const *                                                   \
get_detectors (int & count) pfs_override;

#define PFS_V2_MODULE_DETECTORS_BEGIN(XMOD)                                    \
XMOD::detector_mapper_pair const *                                             \
XMOD::get_detectors (int & count)                                              \
{                                                                              \
	static detector_mapper_pair __detector_mapper[] = {

#define PFS_V2_MODULE_DETECTORS_INLINE_BEGIN                                   \
detector_mapper_pair const *                                                   \
get_detectors (int & count) pfs_override                                       \
{                                                                              \
	static detector_mapper_pair __detector_mapper[] = {

#define PFS_V2_MODULE_DETECTORS_END		                                       \
	};                                                                         \
	count = sizeof(__detector_mapper)/sizeof(__detector_mapper[0]);            \
	return & __detector_mapper[0];                                             \
}

namespace pfs {
namespace app {

template <PFS_MODULUS_TEMPLETE_SIGNATURE>
inline void modulus<PFS_MODULUS_TEMPLETE_ARGS>::module::register_thread_function (thread_function tfunc)
{
    _pdispatcher->_runnable_modules.push_back(pfs::make_pair(this, tfunc));
}

//template <PFS_MODULUS_TEMPLETE_SIGNATURE>
//void modulus<PFS_MODULUS_TEMPLETE_ARGS>::module::register_log_consumer (logger::priority priority
//        , void (module::* log_consumer)(module const *, datetime const &, string_type const &))
//{
//    switch (priority.value) {
//    case logger::priority::debug:
//        _pdispatcher->_emit_debug.connect(this, log_consumer);
//        break;
//    case logger::priority::info:
//        _pdispatcher->_emit_info.connect(this, log_consumer);
//        break;
//    case logger::priority::warn:
//        _pdispatcher->_emit_warn.connect(this, log_consumer);
//        break;
//    case logger::priority::error:
//    case logger::priority::fatal:
//        _pdispatcher->_emit_error.connect(this, log_consumer);
//        break;
//    default:
//		break;
//    }
//}

//template <PFS_MODULUS_TEMPLETE_SIGNATURE>
//void modulus<PFS_MODULUS_TEMPLETE_ARGS>::module::unregister_log_consumer (logger::priority priority
//        , void (module::* log_consumer)(module const *, datetime const &, string_type const &))
//{
//    switch (priority.value) {
//    case logger::priority::debug:
//        this->_pdispatcher->_emit_debug.disconnect(this, log_consumer);
//        break;
//    case logger::priority::info:
//        this->_pdispatcher->_emit_info.disconnect(this, log_consumer);
//        break;
//    case logger::priority::warn:
//        this->_pdispatcher->_emit_warn.disconnect(this, log_consumer);
//        break;
//    case logger::priority::error:
//    case logger::priority::fatal:
//        this->_pdispatcher->_emit_error.disconnect(this, log_consumer);
//        break;
//    default:
//		break;
//    }
//}


}}
#endif /* __PFS_V2_MODULE_HPP__ */
