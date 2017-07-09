/*
 * sigslot_mapping.hpp
 *
 *  Created on: Feb 21, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */


#ifndef __PFS_APPLICATION_SIGSLOT_MAPPING_HPP__
#define __PFS_APPLICATION_SIGSLOT_MAPPING_HPP__

#include <pfs/sigslot.hpp>
#include <pfs/traits/sequence_container.hpp>
#include <pfs/traits/stdcxx/vector.hpp>
#include <pfs/application/module.hpp>

namespace pfs {
namespace application {

class sigslot_mapping_t
{
public:
	virtual ~sigslot_mapping_t() {}
	virtual void connect_all () = 0;
	virtual void disconnect_all () = 0;
	virtual void append_emitter (emitter *e) = 0;
	virtual void append_detector (module * m, detector d) = 0;
};

template <typename EmitterT, typename _detectorT>
struct sigslot_mapping_base : public sigslot_mapping_t
{
    typedef traits::sequence_container<EmitterT *
        , traits::stdcxx::vector>                    emitter_sequence;

    typedef traits::sequence_container<detector_pair
     , traits::stdcxx::vector>                       detector_sequence;

	emitter_sequence  emitters;
	detector_sequence detectors;

	virtual void connect_all ()
    {
        if (emitters.size() == 0 || detectors.size() == 0)
            return;

        typename emitter_sequence::const_iterator itEnd = emitters.cend();
        typename detector_sequence::const_iterator itdEnd = detectors.cend();

        for (typename emitter_sequence::const_iterator it = emitters.cbegin(); it != itEnd; it++) {
            for (typename detector_sequence::const_iterator itd = detectors.cbegin(); itd != itdEnd; itd++) {
                EmitterT * em = *it;
                em->connect(itd->_module, reinterpret_cast<_detectorT> (itd->_detector));
            }
        }        
    }
    
	virtual void disconnect_all ()
    {
        typename emitter_sequence::const_iterator itEnd = emitters.cend();

        for (typename emitter_sequence::const_iterator it = emitters.cbegin(); it != itEnd; it++) {
            EmitterT * em = *it;
            em->disconnect_all();
        }
    }

	virtual void append_emitter (emitter * e)
	{
		emitters.push_back(reinterpret_cast<EmitterT*>(e));
	}

	virtual void append_detector (module * m, detector d)
	{
		detectors.push_back(detector_pair (m, d));
	}
};

/*template <template typename notused = NULL>*/
struct sigslot_mapping0_t : public sigslot_mapping_base<signal0<>, void (module::*)()> {};

template <typename a0>
struct sigslot_mapping1_t : public sigslot_mapping_base<signal1<a0>, void (module::*)(a0)> {};

template <typename a0, typename a1>
struct sigslot_mapping2_t : public sigslot_mapping_base<signal2<a0, a1>, void (module::*)(a0, a1)> {};

template <typename a0, typename a1, typename a2>
struct sigslot_mapping3_t : public sigslot_mapping_base<signal3<a0, a1, a2>, void (module::*)(a0, a1, a2)> {};

template <typename a0, typename a1, typename a2, typename a3>
struct sigslot_mapping4_t : public sigslot_mapping_base<signal4<a0, a1, a2, a3>, void (module::*)(a0, a1, a2, a3)> {};

template <typename a0, typename a1, typename a2, typename a3, typename a4>
struct sigslot_mapping5_t : public sigslot_mapping_base<signal5<a0, a1, a2, a3, a4>, void (module::*)(a0, a1, a2, a3, a4)> {};

template <typename a0, typename a1, typename a2, typename a3, typename a4, typename a5>
struct sigslot_mapping6_t : public sigslot_mapping_base<signal6<a0, a1, a2, a3, a4, a5>, void (module::*)(a0, a1, a2, a3, a4, a5)> {};

template <typename a0, typename a1, typename a2, typename a3, typename a4, typename a5, typename a6>
struct sigslot_mapping7_t : public sigslot_mapping_base<signal7<a0, a1, a2, a3, a4, a5, a6>, void (module::*)(a0, a1, a2, a3, a4, a5, a6)> {};

template <typename a0, typename a1, typename a2, typename a3, typename a4, typename a5, typename a6, typename a7>
struct sigslot_mapping8_t : public sigslot_mapping_base<signal8<a0, a1, a2, a3, a4, a5, a6, a7>, void (module::*)(a0, a1, a2, a3, a4, a5, a6, a7)> {};

}} // pfs::application

#endif /* __PFS_APPLICATION_SIGSLOT_MAPPING_HPP__ */
