/* -*- c++ -*- */
/*
 * Copyright 2011 Free Software Foundation, Inc.
 * 
 * This file is part of gr-eventstream
 * 
 * gr-eventstream is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * gr-eventstream is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with gr-eventstream; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_EVENTSTREAM_SINK_H
#define INCLUDED_EVENTSTREAM_SINK_H

#include <gr_sync_block.h>
#include <gruel/pmt.h>
#include <es/es_queue.h>
#include <es/es_event_loop_thread.hh>
#include <es/es_eh_pair.hh>
#include <boost/lockfree/fifo.hpp>
#include <semaphore.h>

#include <gr_top_block.h>

class es_sink;
using namespace pmt;

typedef boost::shared_ptr<es_sink> es_sink_sptr;

es_sink_sptr es_make_sink (pmt_t arb, es_queue_sptr queue, gr_vector_int insig, int n_threads);

class es_sink : public gr_sync_block
{
private:
  friend es_sink_sptr es_make_sink (pmt_t arb, es_queue_sptr queue, gr_vector_int insig, int n_threads);

  es_sink (pmt_t arb, es_queue_sptr queue, gr_vector_int insig, int n_threads);  	// private constructor


 public:
  ~es_sink ();	// public destructor

  int work (int noutput_items,
	    gr_vector_const_void_star &input_items,
	    gr_vector_void_star &output_items);

  pmt_t arb;
  es_queue_sptr event_queue;
  unsigned long long d_time;
  unsigned int d_history;
  int n_threads;

  boost::atomic<int> d_nevents;
  void wait_events();
//  void wait_events(gr_top_block_sptr tb);

//  sem_t thread_notify_sem;
  boost::condition qq_cond;

  boost::lockfree::fifo<es_eh_pair*> qq;
  boost::lockfree::fifo<unsigned long long> dq;

  std::vector<boost::shared_ptr<es_event_loop_thread> > threadpool;
  std::vector<unsigned long long> live_event_times;


};

#endif /* INCLUDED_EVENTSTREAM_SQUARE2_FF_H */