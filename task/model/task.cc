/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "task.h"

namespace ns3 {
    
        Task::Task()
        {}

        void Task::ini_task_cpu(double cpu)
        {
            task_cpu=cpu;
        }
        void Task::ini_task_mem(double mem)
        {
            task_mem=mem;
        }
        void Task::ini_task_reward(double reward)
        {
            task_reward=reward;
        }
        double Task::get_task_reward() const
        {
            return task_reward;
        }
        void Task::ini_WCET(double Freq,double BU)
        {
            //要详细修改的部分
        }
        double Task::get_WCET() const
        {
            return WCET;
        }
        double Task::get_period() const
        {
            return period;
        }
        void Task::update_trans_delay(double time)
        {
            transmation_delay = transmation_delay + time;
        }
        double Task::get_trans_delay()
        {
            return transmation_delay;
        }
            

}

