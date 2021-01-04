/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef EDGE_DEVICE_H
#define EDGE_DEVICE_H
#include"ns3/core-module.h"
#include"ns3/network-module.h"
#include"ns3/task-module.h"
#include<cmath>
namespace ns3
{
    class edge_device
    {
        public:
            edge_device();
            edge_device(double cpu,double mem);  
            void ini_hardware(double cpu,double mem,double cpu_frequency);
            void ini_position(double x,double y,double z);
            void get_position(double &x,double &y,double &z) const;
            void ini_node(Ptr<Node> node);
            bool is_node(Ptr<Node> node);
            Ptr<Node> get_node();
            double change_trust_model();
            double get_trust_model() const;
            double get_strategy() const;
            double payoff(Task task_temp,int edge_num,double a,double b);
            double remain_cpu() const;
            double remain_mem() const;
            int allocate_cpu(double cpu_needed);
            int allocate_mem(double mem_needed);
            double ini_average_power(Task one_task,double a,double b);
            double cost(Task task_temp,double a,double b);
            void pick_task(Task total_task[]);
            

        
        private:
            void change_strategy(double new_strategy); 
            Ptr<Node> this_node;
            double cpu_frequency;
            double cpu_total;
            double cpu_remain;
            double mem_total;
            double mem_remain;
            double x_index;
            double y_index;
            double z_index;
            double strategy; //因为每个edge device都只挑选一个task所以这里使用double变量来存储，例如:2.000001表明我是第二号(NodeContainer中的)结点，我挑选第五号任务（结点和任务都是从零开始）
            double power_comp;//power consumption for computation
            double power_trans;//power for data transmission
            double average_power;//average power consumption of edge device(通过这个与reward的比较来决定strategy)
            double energy_consumed;
    };
}
#endif /* EDGE_DEVICE_H */

