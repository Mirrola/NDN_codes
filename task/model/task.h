/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef TASK_H
#define TASK_H
#include"ns3/core-module.h"
namespace ns3 {
    
    class Task
    {
        public:
            Task();
            void ini_task_cpu(double cpu);
            void ini_task_mem(double mem);
            void ini_task_reward(double reward);
            double get_task_reward() const;
            void ini_WCET(double edge_cpu,double edge_mem);//Freq is the CPU frequency, BU describes the background CPU utilization(这里我先粗略计算了，先实现大体的功能，后面再详改
            double get_WCET() const;
            double get_period() const;
            void update_trans_delay(double time);
            double get_trans_delay();
            

        private:
            double task_cpu;
            double task_mem;
            double task_reward;
            double WCET=0.2;    //worst-case execution time(先预估为0.2s)
            double period = 0.3;//predefined period Δ where each task,should be processed before the next period starts,
            double input_data_volume;//未用
            double output_data_volume;//未用
            double transmation_delay;//未用

    };

/* ... */

}

#endif /* TASK_H */

