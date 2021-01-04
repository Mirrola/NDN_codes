/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "edge-device.h"


namespace ns3 {
    //public
    edge_device::edge_device()
    {}
    
    edge_device::edge_device(double cpu,double mem)//初始化结点的硬件资源量
    {
        cpu_total = cpu;
        mem_total = mem;
    }
    void edge_device::ini_hardware(double cpu,double mem,double frequency)
    {
        cpu_total = cpu_remain =cpu;
        mem_total = mem_remain =mem;
        cpu_frequency = frequency;
    }

   void edge_device::ini_position(double x,double y,double z) //初始化结点的坐标
    {
        x_index = x;
        y_index = y;
        z_index = z;
    }
   void edge_device::get_position(double &x,double &y,double &z) const 
   {
      x = x_index;
      y = y_index;
      z = z_index;
   }
    void edge_device::ini_node(Ptr<Node> node) //将edge device同node结点建立连接
    {
        this_node = node;
    }
    bool edge_device::is_node(Ptr<Node> node) 
    {
       if(node==this_node)
           return 1;
       else
           return 0;
    }
    Ptr<Node> edge_device::get_node()
    {
        return this_node;
    }
    double edge_device::get_strategy() const
    {
        return strategy;
    }
    double edge_device::payoff(Task task_temp,int edge_num,double a,double b) 
    {
        double pay;
        pay = (task_temp.get_task_reward()*(task_temp.get_period()-(task_temp.get_trans_delay()+task_temp.get_WCET())))/(task_temp.get_period()*edge_num*cost(task_temp,a,b));
        return pay;
    }
    double edge_device::remain_cpu() const
    {
        return cpu_remain;
    }
    double edge_device::remain_mem() const
    {
        return mem_remain;
    }
    int edge_device::allocate_cpu(double cpu_needed)
    //对本结点对任务的承受能力进行级别划分
    {
        if(cpu_needed>cpu_remain)
        {
            return 0;    //0表示无法分配
        }
        else if(cpu_needed==cpu_remain)
        {
            cpu_remain=cpu_remain-cpu_needed;
            return 1;    //1表示需要分配全部cpu
        }
        else if(cpu_needed>=cpu_remain*0.7)
        {
            cpu_remain=cpu_remain-cpu_needed;
            return 2;    //2表示需要分配至少70%cpu
        }
        else if(cpu_needed>=cpu_remain*0.4)
        {
            cpu_remain=cpu_remain-cpu_needed;
            return 3;    //3表示需要分配的cpu占本结点的（40%，70%）
        }
        else  
        {
            cpu_remain=cpu_remain-cpu_needed;
            return 4;  //4表示需要分配的cpu占本结点的（0%，40%）
        }
    }
    int edge_device::allocate_mem(double mem_needed)
    //对本结点对任务的承受能力进行级别划分
    {
        if(mem_needed>mem_remain)
        {
            return 0;    //0表示无法分配
        }
        else if(mem_needed==mem_remain)
        {
            mem_remain=mem_remain-mem_needed;
            return 1;    //1表示需要分配全部cpu
        }
            else if(mem_needed>=mem_remain*0.7)
        {
            
            mem_remain=mem_remain-mem_needed;
            return 2;    //2表示需要分配至少70%cpu
        }
        else if(mem_needed>=mem_remain*0.4)
        {
            mem_remain=mem_remain-mem_needed;
            return 3;    //3表示需要分配的cpu占本结点的（40%，70%）
        }
        else            
        {
            mem_remain=mem_remain-mem_needed;
            return 4;    //4表示需要分配的cpu占本结点的（0%，40%）
        }
    }
    double edge_device::ini_average_power(Task one_task,double a,double b) //a,b是限制因素vi
    {
        power_comp = a*pow(cpu_frequency,b); //要详细修改
        //transmation delay要通过包来获取吗？我的想法：每到达一个结点，便利用打tag的方式修改 delay，将链路的delay进行累加
        power_trans = one_task.get_trans_delay();
        average_power = power_comp + power_trans;
        return average_power;
    }
    double edge_device::cost(Task task_temp,double a,double b) 
    {
        return task_temp.get_WCET()*ini_average_power(task_temp,a,b);
    }



    //private
    void edge_device::change_strategy(double new_strategy)
    {
        strategy=new_strategy;   
    }

}

