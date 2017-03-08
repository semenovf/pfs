/*
 * fsm/exec.hpp
 *
 *  Created on: Jul 26, 2013
 *      Author: wladt
 */

#ifndef __PFS_FSM_EXEC_HPP__
#define __PFS_FSM_EXEC_HPP__

namespace pfs { namespace fsm {

template <typename Sequence>
typename fsm<Sequence>::result_type
fsm<Sequence>::exec (int state_cur
        , const_iterator begin
        , const_iterator end)
{
	const_iterator ptr = begin;
	const_iterator ptr_accepted = begin;
	
	bool accepted = false;

	// FIXME no need after _contex
//	PFS_ASSERT(_context);
//	PFS_ASSERT(_context->_trans_tab);

    // FIXME need to check state_cur value (must be in specified bounds)
    transition const * trans = & _pcontext->trans_tab[state_cur];

	do {
		result_type result = trans->m(_pcontext.get(), ptr, end);

		if (result.first) {
			if (trans->action) {
				if (!trans->action(ptr
                        , result.second
                        , _pcontext->parse_context
                        , trans->action_args)) {

					// Let's support this situation
					//
					//static FSM_TRANSITION record_fsm[] = {
					//	  {-1,  1, FSM_MATCH_SEQ(10) , FSM_ACCEPT, on_record1, 0 }
					//	, {-1,  2, FSM_MATCH_SEQ(11) , FSM_ACCEPT, on_record2, 0 }
					//	, {-1, -1, FSM_MATCH_SEQ(12) , FSM_ACCEPT, on_record3, 0 }
					//};
					if (trans->status == accept_status) {
						if (trans->state_fail >= 0) {
							trans = & _pcontext->trans_tab[trans->state_fail];
							continue;
						}
					}

					return result_type(false, end);
				}
			}

			if (trans->status == accept_status) {
				accepted = true;
			}

			ptr = result.second;

			if (trans->status == accept_status) {
				ptr_accepted = ptr;
			}

			if (trans->status == reject_status) {
				state_cur = -1;
				accepted = false;
			} else {
				state_cur = trans->state_next;
			}
		} else {
			state_cur = trans->state_fail;

			if (trans->status != accept_status) {
				accepted = false;
			}

			ptr = ptr_accepted;
		}

		if (state_cur < 0)
			break;

		trans = & _pcontext->trans_tab[state_cur];

	} while (true);

	return accepted
			? result_type(true, ptr_accepted)
			: result_type(false, end);
}

}} // pfs::fsm

#endif /* __PFS_FSM_EXEC_HPP__ */
