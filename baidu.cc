/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */


//106


#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/netanim-module.h"
#include "ns3/energy-module.h"
//#include "basic-energy-sourc.h"
// Default Network Topology
//
// Number of wifi nodes can be increased up to 250
//                          |
//       Rank 0 
// ------------------
//   Wifi 10.1.1.0
//                 AP
//  *    *    *    *
//  |    |    |    |   
// n3   n2   n1   n0 

using namespace ns3;

void MacTxCallback(std::string context,Ptr<const Packet> packet)
{
    /*Ptr<BasicEnergySource> basicSourcePtr = DynamicCast<BasicEnergySource> (cpu_have_all.Get (0));
    double b= basicSourcePtr->GetRemainingEnergy(); */
    NS_LOG_UNCOND("+"<<Simulator::Now().GetSeconds()<<" "<<"i have "<<" cpu");
}


NS_LOG_COMPONENT_DEFINE ("mywireless");
void RemainingEnergy (double oldValue, double remainingEnergy)
 {
   NS_LOG_UNCOND (Simulator::Now ().GetSeconds ()
                  << "s Current remaining energy = " << remainingEnergy );
 }


/*------------------配置trace变量以查看结点当前的energe-------------------*/
/*void PrintRemainingEnergy(EnergySourceContainer cpu_sources,int nWifi)
{
    Ptr<BasicEnergySource> basicSourcePtr_cpu;
    double b;
    Ptr<BasicEnergySource> basicSourcePtr_mem;
    double c; 
 
    for(int i=1;i<nWifi+1;i++)
    {
        basicSourcePtr_cpu = DynamicCast<BasicEnergySource> (cpu_sources.Get (2*i-1));
        b= basicSourcePtr_cpu->GetRemainingEnergy(); 
        basicSourcePtr_mem = DynamicCast<BasicEnergySource> (cpu_sources.Get (2*i));
        c= basicSourcePtr_mem->GetRemainingEnergy(); 
      // std::cout<<b<<std::endl;
        NS_LOG_UNCOND("第"<<i<<"个结点申请的cpu资源数为"<<b);
        NS_LOG_UNCOND("第"<<i<<"个结点申请的mem资源数为"<<c);
        basicSourcePtr_cpu->~Ptr();
        basicSourcePtr_mem->~Ptr();
    }
}//通过EnergySourceContainer来查找相应的energy sourrce
*/
void PrintRemainingEnergy_node(NodeContainer wifiStaNodes,int nWifi)
{
    //当接受结点接受到一个包的时候，可以通过ip地址反向得到相应结点
    //现在希望通过node结点来访问到他的energy source的信息
    Ptr<Node> now_node = wifiStaNodes.Get(0);
    EnergySourceContainer contai;
    //TypeId tid = contai.GetInstanceTypeId();
 
    Ptr<EnergySourceContainer> EnergySourceContainerOnNode = now_node->GetObject<EnergySourceContainer>();
    Ptr<BasicEnergySource> EnergySourceOnNode_cpu= DynamicCast<BasicEnergySource> (EnergySourceContainerOnNode->Get(0));
    double b = EnergySourceOnNode_cpu->GetRemainingEnergy();

    
    Ptr<BasicEnergySource> EnergySourceOnNode_mem= DynamicCast<BasicEnergySource> (EnergySourceContainerOnNode->Get(1));
    double c = EnergySourceOnNode_mem->GetRemainingEnergy();
    NS_LOG_UNCOND("当前计算结点持有可用cpu资源量为 "<<b);
    NS_LOG_UNCOND("当前计算结点持有可用mem资源量为 "<<c);
}




/*------------------------------------------------------------------------*/



int 
main (int argc, char *argv[])
{
  bool verbose = true; // 如果为true，则在终端输出日志内容
  uint32_t nWifi = 3; // sta节点个数
  bool tracing = false; // 如果为true，则生成跟踪文件(.pcap)

  CommandLine cmd;
  cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);

  cmd.Parse (argc,argv);

  // Check for valid number of csma or wifi nodes
  // 250 should be enough, otherwise IP addresses 
  // soon become an issue
  if (nWifi > 250)
    {
      std::cout << "Too many wifi nodes, no more than 250 each." << std::endl;
      return 1;
    }

  if (verbose)
    {
      //LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      //LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

  /* 创建wifi的AP节点和Sta节点 */
  NodeContainer wifiApNode;
  wifiApNode.Create (1); // 创建一个AP节点
  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nWifi); // 创建nWifi个sta节点

  /* 一旦创建了这些对象，我们创建一个通道对象并将其关联到我们的PHY层对象管理器，以确保由YansWifiPhyHelper创建的所有PHY层对象 共享相同的底层通道，即它们共享相同的无线介质，并且可以沟通与干涉 */
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default (); //配置Channel助手 
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default (); // 配置phy助手
  phy.SetChannel (channel.Create ()); // 使每一个phy与Channel相关联

  WifiHelper wifi; // 创建wifi助手,有助于创建WifiNetDevice对象
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager"); // 设置wifi助手对象的速率控制算法类型：AARF算法

  // MAC层设置。
  WifiMacHelper mac; // 创建mac层
  Ssid ssid = Ssid ("ns-3-ssid"); // 创建IEEE 802.11 SSID信息元素
  mac.SetType ("ns3::StaWifiMac", // 设置mac类型为"ns3::StaWifiMac"
               "Ssid", SsidValue (ssid), // 设置mac的"Ssid"属性的值为SsidValue (ssid)
               "ActiveProbing", BooleanValue (false)); // 设置mac的"ActiveProbing"属性的值为BooleanValue (false)。如果为true，我们发送探测请求。

  NetDeviceContainer staDevices; // 创建WifiStaNetDevice对象
  staDevices = wifi.Install (phy, mac, wifiStaNodes); // 根据phy、mac和节点集合返回一个NetDeviceContainer对象

  mac.SetType ("ns3::ApWifiMac", // 设置mac类型为"ns3::ApWifiMac"
               "Ssid", SsidValue (ssid));

  NetDeviceContainer apDevices; // 创建WifiApNetDevice对象
  apDevices = wifi.Install (phy, mac, wifiApNode);

  /* 添加移动模型，使sta节点处于移动状态，使ap节点处于静止状态 */
  MobilityHelper mobility; // 创建MobilityHelper对象，将位置和移动模型分配给节点

  // 设置位置分配器，用于分配MobilityModel :: Install中初始化的每个节点的初始位置。
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator", // 设置使用的移动模型的类型(在矩形2d网格上分配位置)
                                 "MinX", DoubleValue (0.0), // 网格开始的x坐标
                                 "MinY", DoubleValue (0.0), // 网格开始的y坐标
                                 "DeltaX", DoubleValue (5.0), // 对象之间的x空间
                                 "DeltaY", DoubleValue (10.0), // 对象之间的y空间
                                 "GridWidth", UintegerValue (3), // 在一行中排列的对象数
                                 "LayoutType", StringValue ("RowFirst")); // 布局类型
  // 设置移动模型类型
  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel", // 该类型：其中节点在随机方向上以随机速度围绕边界框移动
                             "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50))); // 界限属性(矩形的范围)
  mobility.Install (wifiStaNodes); // 把该移动模型安装到wifiStaNodes节点上

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel"); // 固定位置的模型
  mobility.Install (wifiApNode);

  /*-----------------------------------------------------------------*/
    //实际任务的cpu与mem的比重可再调整
    //查查怎么用官网源代码中的类！！！
    BasicEnergySourceHelper cpu_needed;
    cpu_needed.Set("BasicEnergySourceInitialEnergyJ",DoubleValue(40));
    BasicEnergySourceHelper mem_needed;
    mem_needed.Set("BasicEnergySourceInitialEnergyJ",DoubleValue(30));
    EnergySourceContainer cpu_sources=cpu_needed.Install(wifiStaNodes.Get(0));
    EnergySourceContainer mem_sources=mem_needed.Install(wifiStaNodes.Get(0));
    
       
    cpu_needed.Set("BasicEnergySourceInitialEnergyJ",DoubleValue(30));
    mem_needed.Set("BasicEnergySourceInitialEnergyJ",DoubleValue(10));
    EnergySourceContainer cpu_sources_1=cpu_needed.Install(wifiStaNodes.Get(0));
    EnergySourceContainer mem_sources_1=mem_needed.Install(wifiStaNodes.Get(0));
 


// std::cout<<"0结点的cpu为"<<wifiStaNodes.Get(0)
    //接下来就是准确定位单个结点的单个energy，要能在发送的包中加入相关energy的信息

    cpu_needed.Set("BasicEnergySourceInitialEnergyJ",DoubleValue(20));
    mem_needed.Set("BasicEnergySourceInitialEnergyJ",DoubleValue(20));
    EnergySourceContainer cpu_sources_2=cpu_needed.Install(wifiStaNodes.Get(0));
    EnergySourceContainer mem_sources_2=mem_needed.Install(wifiStaNodes.Get(0));

    cpu_needed.Set("BasicEnergySourceInitialEnergyJ",DoubleValue(30));
    mem_needed.Set("BasicEnergySourceInitialEnergyJ",DoubleValue(30));
    EnergySourceContainer cpu_sources_3=cpu_needed.Install(wifiStaNodes.Get(0));
    EnergySourceContainer mem_sources_3=mem_needed.Install(wifiStaNodes.Get(0));
    
    BasicEnergySourceHelper cpu_have;
    cpu_needed.Set("BasicEnergySourceInitialEnergyJ",DoubleValue(40));
    BasicEnergySourceHelper mem_have;
    mem_needed.Set("BasicEnergySourceInitialEnergyJ",DoubleValue(30));
    EnergySourceContainer cpu_have_all=cpu_needed.Install(wifiApNode.Get(0));
    EnergySourceContainer mem_have_all=mem_needed.Install(wifiApNode.Get(0));


 














  /*----------------------------------------------------------------*/

  /* 安装协议栈 */
  InternetStackHelper stack; // 用于将IP / TCP / UDP功能聚合到现有节点
  stack.Install (wifiApNode);
  stack.Install (wifiStaNodes);

  /* 给设备接口分配IP地址 */
  Ipv4AddressHelper address;

  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer apInterfaces;  // apInterfaces用于获取ap节点的ip地址
  apInterfaces = address.Assign (apDevices);
  Ipv4InterfaceContainer staInterfaces; // staInterfaces用于获取sta节点的ip地址
  staInterfaces = address.Assign (staDevices);

  /* 创建clien和server应用 */
  UdpEchoServerHelper echoServer (9); // 创建一个端口号为9的server

  ApplicationContainer serverApps = echoServer.Install (wifiApNode.Get (0)); // 把该server安装到ap节点上
  serverApps.Start (Seconds (1.0)); // server开始于程序运行的第1秒中
  serverApps.Stop (Seconds (10.0)); // server结束于程序运行的第10秒中

  UdpEchoClientHelper echoClient (apInterfaces.GetAddress (0), 9); // 创建client，并为其指向ap节点的ip地址和端口号
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1)); // 应用程序发送的最大数据包数
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0))); // 数据包之间等待的时间
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024)); // 出站数据包中回波数据的大小

  ApplicationContainer clientApps_1 = echoClient.Install (wifiStaNodes.Get(0));
  clientApps_1.Start (Seconds (2.0));
  clientApps_1.Stop (Seconds (10.0));
  ApplicationContainer clientApps_2 = echoClient.Install (wifiStaNodes.Get(1));
  clientApps_2.Start (Seconds (3.0));
  clientApps_2.Stop (Seconds (10.0));

  ApplicationContainer clientApps_3 = echoClient.Install (wifiStaNodes.Get(2));
  clientApps_3.Start (Seconds (4.0));
  clientApps_3.Stop (Seconds (10.0));
  /* 启用互联网络路由 */
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Stop (Seconds (10.0));

  if (tracing == true)
    {
      phy.EnablePcap ("mywireless", apDevices.Get (0));
      phy.EnablePcap ("mywireless", staDevices);
    }

  AnimationInterface anim("mywireless.xml");
  /*--------------------打印结点的energy---------------------------------*/
    //现在要确定trace变量打印的时间，例如first。cc中的，在发送包的时候打印
    //现在应该是只建立起了trace变量而没有确定打印的时间。

 


  /*---------------------------------------------------------------------*/
  // PrintRemainingEnergy(cpu_sources,nWifi);
  PrintRemainingEnergy_node(wifiStaNodes,nWifi);
 // Ptr<Node> now_node = wifiStaNodes.Get(0);
  Simulator::Run ();
  Simulator::Destroy ();
  /* Simulator::Schedule(Seconds(0.0),&NS_LOG_REMAININGENERGY,cpu_sources_1);
   Simulator::Stop (Seconds (10.0));
   Simulator::Run ();
   Simulator::Destroy();*/
  return 0;
}
