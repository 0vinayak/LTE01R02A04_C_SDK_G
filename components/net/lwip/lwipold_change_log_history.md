(8910_master lwip modify the summary)

2018-07-10: tjhaoli
* api_msg.c: (Add the cfw socket api reference.)
commit-ID: 7fd9336e69da581f5aa3160e8fb8bd63aea467e4

2018-10-18: shaoboan(未合入)
* ppp.c: Fixed Bug  (fix ppp stress test crash bug)
commit-ID:1ce6588b8f034b8f8e3381101207b22eb3117554

2018-12-10: Eric Guo(未合入)
* ppp.c: Fixed Bug #975440 (avoid dead lock in PS IPC driver)
There are several dead lock in PS IPC driver:
    * When there are no uplink buffer, write will wait. In case of PPP
      application, both write and read are performed on AT thread. The
      waiting will make AT thread not to read from PS IPC.
    * The PS IPC callback is called with lock hold. When AT thread is busy,
      send event to AT thread may be blocked. And then PS IPC read write
      will be blocked also, due to can't get the lock. Now, callback are
      called without lock. It will be unsafe for PS IPC interface delete.
      So, PS IPC interfaces are changed from create/delete to open/close.
      Event an interface is closed, the pointer itself is valid.
    
    * Also, add a feature to trigger AP assert when IDLE_RES0 is written to
      0xdead. It is useful to debug AT thread infinite loop or dead lock.
    * Enlarge trace buffer from 32KB to 64KB.
    * Some trace cleanup to reduce default trace.
commit-ID: 3bedd5975dd6aa248c8999f97a6b88e2aa74071b

2019-08-06: wei.sun2
* ppp-fsm.c: Fixed Bug #1137129  (Add protect to not run fsm_timeout callback if fsm timer is canceled.)
commit-ID: 279418b495257d677eb912e972f256f1adcddd5a

2018-08-10: shaoboan(未合入)
* ppp.c: Fixed Bug  (add ppp module)
commit-ID: 0667b25e6e137322f08a7321d1b700cc9f937e54

2018-08-14: shaoboan
* ppp.c: Fixed Bug  (modify netif create and netif destory)
commit-ID: 64647fe84a2f2ccbef1ee97e0e168695ba17cce4

2018-08-17: shaoboan(未合入)
* ppp.c: Fixed Bug  ( modify ppp lib , now ppp is ok to connect)
commit-ID: 2d03fd12897f2eb1a2c5c237e311c26a8ccce5ea

2018-09-12: Guo Chao
* api_msg.c: (Replace cos prefix to osi)
commit-ID: 389197d14c16ed7280dd6b3909cfae50a8fa72c1

2018-10-07: Guo Chao
* tcpip.c: Fixed(Cleanup cfw for 8909L)
commit-ID: bb3187162f48f36eca14bb9a55ecb88ae236ba34

2018-10-19: shaoboan
* ppp.c: Fixed Bug  (When ppp_close , netif is freed before pppdestory. Free netif in pppdestory.)
commit-ID: 7031fa48ee9e13c4dcb964bc1d9dae08a47a88b3

2018-10-26:  xiwenmao
* ppp.c: Fixed Bug  (fix long ppp netbuf data read from IPC error)
commit-ID: 5b96d897e31498c17915b28462af884a71f5a76b

2018-10-26: shaoboan
* netif.c,ppp.c/ppp.h,lcp.c: (fix GPRS_DEVICE_IND ppp unclose bug)
issue: When network is invalid , ppp should auto close. solution:Add auto close function
commit-ID: 9bcc0c53f6037f8d7265889b3a1b4fb5c3f6a29d

2018-11-16: jialiu
* sockets.c: Fixed (add mynet tcpip at commands add getmss api)
commit-ID: 7c7cc2e67c9584fd7a73a0f898ba4a5a67e8174c

2018-11-23: Guo Chao
* api_msg.c: (Retire cswtype.h)
commit-ID: 74a7abe7505a528ad116db11248ed41dfd2e44e4

2018-12-12: xiwenmao
* api_lib.c, dns.c: Fixed bug #975615 (fix AT+CDNSGIP return error ip when input host name is IPaddr)
commit-ID：ccfff03febc7981e8c17e7ef558a584d411528f0

2019-01-16: shaobo.an(未合入)
* pbuf.c: Fixed bug #1002576 (add new log)
commit-ID: 01b8fa2753814a81ccd90fbdbfb999bdd159732d

2019-01-16: shaobo.an(未合入)
* pbuf.c: Fixed bug #1002427 (add new log)
commit-ID: 35552ca1898adbb4366c41cc798019213b04526c

2019-01-18: shaobo.an(未合入)
* pbuf.c: Fixed bug #1002576 (modify debug printf)
commit-ID: 582393898b3fa46bbef55f9db15b34face0df8e8

2019-01-18: shaobo.an(未合入)
* pbuf.c: Fixed bug #1002427 (modify debug printf)
commit-ID: c021e1697464d88ea076e2a4393e98b671dd8243

2019-03-18: xiwenmao
* api_lib.c,dns.c,netif.h: Fixed bug #1027175 (add IPV6 DNS and netif in lwip)
commit-ID: 7575fbe62ac37da5d90ccc19daf1c91498644069

2019-03-22: xiwenmao
* icmp.c: Fixed Bug#1032724 (fix crash after receive wrong ICMP package)
commit-ID:269880ff8f615af87e7b2619f9e3dce70f7d73e1

2019-03-22: xiwenmao
* icmp.c: Bug #1032724 (fix crash after receive wrong ICMP package)
commit-ID:32768bc4cc0ba8079293b6caf516ac0cc6bae0d7

2019-04-24: feng.liu
* ppp-fsm.c: Fixed Bug #1044255  (reconnect fail problem)
commit-ID: 53737596c19778e89517da49656ec250e97a5e1c

2019-04-29: xiwenmao
* nd6.c: Bug #1051905 (do not send RS package in test mode)
commit-ID: ba8ee1f4ad99bf6104192bebcedf21680cda5cab

2019-04-29: xiwen.mao(未合入)
* pbuf.c: Fixed bug #1054245 (remove some print for rate test)
commit-ID: d0ebee80f67e539b514b61b4bab8b16397c016a7

2019-05-06: jialiu
* sockets.c/sockets.h: Fixed Bug #1037448 (modify mynetack display when socket is not opened)
commit-ID: 5573682d9278eb7e8ab3c2064de59bc6b084788a

2019-05-06: xiwenmao
* nd6.c:Bug #1064307 (do not deactive pdp in testmode in nd6_tmr)
commit-ID:0794c2f9d25400c56673a3f30894c9c888b3dc70

2019-05-08: xiwenmao
* tcp.c: Fixed Bug #1066224 (return err_code to app if IPV6 route error)
commit-ID: 1b781c011adc7b6bfe15ee702871b278402a92de

2019-05-08:  Xianyong.Cheng
* dns.c: Fixed bug #1063816 (coverity fix OVERRUN issue)
commit-ID: 3265b26282ef905b9584556fa4064c7ea2eb835d

2019-05-08: Xianyong.Cheng
* dhcp.c: Bug #1063818 (coverity fix OVERLAPPING COPY issue)
commit-ID:34e65881eb732d5df682a8f26e64c032cce3177c

2019-05-17: wei.sun2
* ppp-fsm.c: Fixed Bug #1070874  (appanic when pppos_input after ppp_free)
commit-ID: 0e6bf2ff3d4be244912b2e986c5e6b5ed5cc926b

2019-05-18: xiwenmao
* sockets.c: Fixed Bug #1027175 (enable LWIP_IPV6, fix build error)
commit-ID: 69b9311da13eff63d4654d47c7d0e7fa27fe45fb

2019-05-21: xiwenmao
* tcp_out.c: Fixed Bug #1076004 (fix NULL pointer crash in tcp data send)
commit-ID: a7f63d70917a0ea2647f7511cb7c25fba396734f

2019-05-30: xiwenmao
* tcp.c: Fixed Bug #1083660 (Add core locking for tcp timer callback)
commit-ID: d17ea5e55e9ea396e1e17191043a61363639f6cc

2019-06-14: xiwenmao
* dns.c: Fixed bug #1096062 (return err immediately in DNS if no netif)
commit-ID: 5c7ba959df5272402e04a45f09c2ff5b89753e7c

2019-07-01: xiwenmao
* dns.c: Fixed bug #1107467 (fix dns crash when get dns package from different dns server with default server)
commit-ID: bec7010226c8dd1e427f594f34a777112445ff7c

2019-07-01: jialiu
* api_msg.c: Fixed bug #1103487 (change order between send rsp and release signal)
commit-ID: 10fb2860e9c0df72b075c072c73c67dbd23b4bb4

2019-07-05: cui.liangjin
* ppp.c: Fixed Bug #1096838  (lcpEchoRequset send in at thread)
commit-ID: 165f8e03bed32487270d31618eef8596c8b9d80f

2019-07-09: xiwenmao
* nd6.c: Bug #1112829 (improve ipv6 prefix get speed)
commit-ID: b0c84e60fac15d8ff4303fafc8ad2eb7925eb3ad

2019-07-09: xiwenmao
* api_lib.c,netif.c: Fixed bug #1112939 (fix dns get wrong type ip_addr when active multi-PDP)
commit-ID: 87c6862a2e20fb54f7b520332d26fc3746a4dd94

2019-07-21: wei.sun2
* ppp-fsm.c: Fixed Bug #1120733  (Add protect to not run fsm_timeout callback if fsm timer is canceled.)
commit-ID: 37cf069cf5c66442701d90a04cf3414a7d9cb2a7

2019-07-26: xiwen.mao
* netif.c: Fixed bug #1094871 (do not set loopback netif to default netif)
commit-ID: 1b37b2c4a637c2788016a8d2972233295eb0c23e

2019-08-06: Xianyong.Cheng
* ip6_addr.c: Fixed Bug#1136465 (fix coverity scan null pointer risk issue)
commit-ID:9d67f8f6bbbde41dfd8c6ac0cf199533b94aeaf1

2019-08-09: howie.li
* netdb.c: Fixed Bug #1136463 (Fix the converity code scan issue of cid 39254.)
commit-ID: ea37ea7c0d8435ae17044a1e47ece4d20c9b61fa

2019-08-09: howie.li
* tcp_in.c: Fixed Bug #1136467 (Fix the converity code scan issue of cid 39384.newlib alrerady fix)
commit-ID: 3ef3505bc858c6299191c0192c5be30449d670ee

2019-08-09: xiwenmao
* nd6.c: Bug #1136466 (Fix the converity code scan issue of cid 39760;39695.)
commit-ID: ef5cd02f09fc4e23c72a4a3adecde3b4a6dd3370

2019-08-09: howie.li
* nd6.c: Fixed Bug#1136466 (Fix the converity code scan issue of cid 39760;39695.)
commit-ID:af2a0e45ae80e6d0ebd4d573481979fef344330c

2019-08-13: jialiu
* api_msg.c,sockets.c/sockets.h: Fixed bug #1136672 (add ipv6server support.)
commit-ID: fa591130e90973652b7e779061e5edf028a6b413

2019-09-04: jialiu
* tcp.c: Fixed Bug #1157424 (initiallize lpcb->netif_idx when listen)
commit-ID: 2fa166165a9b81c76a22c79a1da08d85ecdf717f

2019-09-20: jialiu
* netif.c: Fixed bug #1170183 (midify mynetact read addr api)
commit-ID: 357b573faa580d72fd1de359c6cc9b9591788611

2019-09-24: Xianyong.Cheng
* api_lib.c: Fixed bug #1172034 (fix ipv6 domain http get failed issue dns query incorrect)
commit-ID: f5ff7bfe204e82942a94442f2df93b541f7836f0

2019-10-10: jialiu
* ipv6.c: Bug #1175896 (return null when netif is loopback and the dest is a normal addr)
commit-ID:50bec638ca16349f4c08abaa7cc7ed074a7a2c71

2019-10-15: xiwenmao
* netif.c: Fixed bug #1173613 (Add multi-apn support)
commit-ID: d3b3a11a2a58777e74e19534f97c5ff32b06223b

2019-11-16: wei.sun2
* ppp-fsm.c: Fixed Bug #1124092  (remove globle var for support multi-ppp)
commit-ID: 471952bfecd3d416a4963238abc5340c939c1309

2019-11-31: xiwenmao
* sockets.c/sockets.h: Fixed Bug #1186893 (fix NULL pointer crash)
commit-ID: 15666ac7e8a7cbdfb5a366921132eed9d9bae0cb

2019-12-03: jialiu
* tcp.c: Fixed Bug #1214316 Bug #1215000 (add listen socket recv bearing loosing event and add timeout in mynetopen)
commit-ID: 1aa6bf1c170a9b3ac72fdf1a5f4962dbac965f48

2019-12-12: jialiu
* api_msg.c: Fixed bug #1211720 (modify accept err abort.)
commit-ID: 7a5c999384d42ca6cd5bf824c4679d53f0936f24

2019-12-17: xiwenmao
* sockets.c: Fixed Bug #1211024 (send data or close event if socket get data or closed by remote server before accept)
commit-ID: 6c74c032ed1c36dcaad6079bc53a16508357a319

2020-01-02: xiwenmao
* tcp.c: Fixed Bug #1354324 (add rng to generate random local port and ipid when bootup)
commit-ID: 243c8f3f1c88a0fca6be0c766eed27f25337715a

2020-01-07: xiwenmao
* tcp_out.c,tcp_in.c,tcp.c/tcp.h: Fixed Bug #1234271 (fix TCP timer not correct)
commit-ID: db0e8aaa33bb0d00942be970d26e7a2e50835313

2020-01-13: xiwenmao
* sockets.c: Fixed Bug #1211024 (change EV_CFW_TCPIP_REV_DATA_IND from event to notify)
commit-ID: 474fbf018049dd0eec1c9b39a74e7a8c06c1854f

2020-01-14: howie.li
* sockets.c: Fixed Bug #1211024 (Use the notify method to get data from lwip.)
commit-ID: e8ea1c5491789322d5696d9aae99089e2c273528

2020-02-17: fei.pan(未合入)
* sockets.c,tcp.c,udp.c...: Fixed Bug #1251375 (cut lwip log in 8811(amend X8_F to 02x..))
commit-ID: b112d518cd00ce5e65cba735c5aae6f9bfd526ee

2020-02-18: guangzu.sun(未合入)
* sockets.c,tcp.c,udp.c...: Fixed Bug #1251393 (modify %s printf add lwip debug format)
commit-ID: a6fa3f8867683581b866ad4bf76b164d1c28e562

2020-02-19: xiwenmao(未合入)
* netdb.c: Fixed Bug #1251375 (cut lwip log in 8811(amend %s and format to raw.c))
commit-ID: eb9aae6ad1900e1bada65731d133e72a52d3fd19

2020-02-22: guangzu.sun(未合入)
* sockets.c,tcp.c,udp.c...: Fixed Bug #1251393 (cut lwip debug module and add traceid)
commit-ID: 4c78b3a40d6a0d13c500bf8f908d7015695497cd

2020-02-25: xiwenmao(未合入)
* sockets.c,tcp.c,udp.c...: Fixed Bug #1251393 ( change net debug string to traceID)
commit-ID: d619a445cf93aca7b1059ffc18c5fcbbf2379c92

2020-03-18: jialiu
* altcp_tls.c: Fixed  Bug #1275113 (lwip_mqtts add altcp_mbedtls)
commit-ID: 06d7a3a1efad070e11c7d15d6d67543163f49b96

2020-04-15: fei.pan
* ipv6.c: Bug #1298297 (at+ ping ipv6 failed after ppp)
commit-ID:aaccf4b43796cfc7c4bd63579862abc248af4cbb

2020-04-13: jialiu
* sockets.c: Fixed Bug #1295231 (modify msg peek while)
commit-ID: 1db7b7b6d2ab6eb0586a09b2bef90cbc870392f1

2020-04-11: wei.sun2
* upap.c,ppp.c,auth.c: Fixed Bug #1285846 (add pap chap support for ppp dail)
commit-ID: 922c9a4c94f67d7ba90e97454a287bd9b634c956

2020-04-15: jialiu
* altcp_tls.c: Fixed  Bug #1275113 (lwip_mqtts add altcp_mbedtls)
commit-ID: 99c552a2ca5e85d018e258491b21d72e31f33e04

2020-04-16: wei.sun2
* sockets.c: Fixed Bug #1285846 (bug fix PAP CHAP for fibocom)
commit-ID: c9112548f7cb01087854f0be10af51f04001d9f2

2020-04-21: howie.li
* api_msg.c,sockets.c: Fixed bug #1299057 (Fix the download  data is not full when run ftpget.)
commit-ID: 4add08f0cd2f2bd3ea38882bb34e2b58a04fdd4a

2020-04-22: jialiu
* dns.c: Fixed bug #1258641 (initial ip6_addr_t when get dns ipaddr)
commit-ID: b83969eb9df3465b09a181327361c15fe0e542ca

2020-04-28: xiwen.mao
* udp.c,tcp.c/tcp.h: Fixed bug #1268700(Add NAT support)
commit-ID: 8df61c0a4fa70ecc25f8a2bc3d1595a5e07547b2

2020-05-23: jialiu
* api_msg.c: Fixed bug #1271707 (initial lpcb and do not tcp_abort when close listen socket to fix crash.)
commit-ID: 8aaa595a8c8d1a42ce03f73a616ff755283afead

2020-06-01: xiwenmao
* sockets.c,api_msg.c: Fixed Bug #1330985 (fix null pointer crash in lwip_send)
commit-ID: 7a1f32b3d03819c5dec48e2f0b7de9556183e513

2020-06-02: xiwenmao
* sockets.c: Fixed Bug #1325976 (add tcp lock to protect multi-thread access)
commit-ID: a8f09272b551b3e02343ab91c4ef485efcf94771

2020-06-02: xiwenmao
* api_msg.c,sockets.c: Fixed Bug #1334252 (fix NULL pointer crash)
commit-ID: aa6d1d828029b476e7179b73211c552c21ae939b

2020-06-04: fei.pan
* api_lib.c,dns.c udp.c: Fixed bug #1331295 (AT/NET thread are out of sync,double free in dns reslove,causing crash)
commit-ID: 29fb66ec303a4dd3226b7ed2c38002c6884ad38f

2020-06-04: fei.pan
* api_lib.c,dns.c udp.c: Fixed bug #1331295 (AT/NET thread are out of sync,double free in dns reslove,causing crash)
commit-ID: eef555c282c9e065c14127360afeb6eee1660a8f

2019-06-12: xiwenmao
* nd6.c: Bug #1094062 (Do not deactive PDP if can not get IPV6 prefix)
commit-ID: c4473eda29bae180e4df2d1a01cd174d932b9305

2020-07-07: fei.pan
* dns.c: Fixed bug #1356412 (using at+cdu active pdp,can not ping)
commit-ID: ee62009e54d99b5d70e399b85182fab8f16f3442

2020-07-07: fei.pan
* ip4.c: Fixed Bug# 1359776 (rndis active pdp,ping IPv4 address not show error)
commit-ID:50b13abf697f8c1747fbb43363955fda77a57fa9

2020-07-09: xiwen.mao
* ipv6.c: Bug #1362764 (add NET_LWIP_IPV6_ON in menuconfig)
commit-ID:20cca3e40c0f977c68e50af269a2d393e34d9632

2020-07-11: xiwen.mao
* dns.c: Fixed bug #1363911 (fix NULL pointer crash in dns_send)
commit-ID: 9321d5831167e36c42fa3b5dc6f22aef51107bda

2020-07-15: xiwenmao
* upap.c: Fixed Bug #1365717 (fix RNDIS NAT download data size error)
commit-ID: 8fe433508297f4b2ab7dae1c8eb52de8d946bc77

2020-07-15: fei.pan
* ipv6_addr.c: Bug #1365831 (cfgimspdnmi read ipv6 address is not same with the set param)
commit-ID:a0afee3a8bdf3560992bf9edeb335f7aba74a4e2

2020-07-18: xiwenmao
* ip4_frag.c.c: Fixed Bug #1349812 (fix lwip udp fragment out-of-order bug)
commit-ID: 35faa122a459e43d98252dcf579f79bda93885d8

2020-08-05: xiwenmao(未合入)
* sockets.c: Fixed Bug #1232774 (disable lwip_tcp and tcp application on 8811)
commit-ID: 7c55b974bec2b19f1c16f84641f45701265075dc

2020-08-05: xiwenmao
* tcp_out.c/ip_nat.c: Fixed Bug #1433539 (fix TCP resend package NAT port error)
commit-ID: 9ed0c00c84af7206d27c25c860d09190cd53e8e5

2019-08-09: jialiu
* ipv6.c: Bug #1136464 (add null protect in ipv6.c ip6_input)
commit-ID:2f54af6bf2f5cece221643347603d500b7242bd0

2019-08-19: xiwenmao
* ipv6.c: Bug #1144709 (fix coverity bug, add NULL pointer protect)
commit-ID:b0bd389419eed52da509ccff407ec10a15182984

2019-08-29: xiwenmao
* ipv6.c: Bug #1144709 (fix coverity bug, add NULL pointer protect)
commit-ID:fddee722027cb76181ae89ddd0513ed290d3add9

2020-09-03: fei.pan
* dns.c: Fixed bug #1402597 (fix dns_send complexity)
commit-ID: 161bc4a19507079dbefea080008efb17d8625efa

2020-09-11: xiwenmao
* netif.c: Fixed bug #1403803 (add static route by dest ip)
commit-ID: aa425d13dc6c29c5f21a9d2a0feefd737eec4180

2020-09-14: fei.pan
* api_lib.c,dns.c udp.c: Fixed bug #1411519 (amend dns resolve in different pdp type)
1. 当netif！=NULL时，激活IPv4时，只进行IPv4解析；激活IPv6时，只进行IPV6解析；激活IPv6v4时，先进行IPV6,再进行IPV4;
2. 当netif == NULL时，默认先进行IPV4解析，再进行IPV6;
3. 当底层lwip协议栈不支持双栈时，进行IPv4解析,修改为进行地址绑定时，将绑定的地址类型改为V4V6。
commit-ID: 46fcb77a581e3847835923bb62215d2563a34f47

2020-09-15: xiwenmao
* nd6.c: Bug #1402403 (add nd6_get_router_addr interface)
commit-ID: 33f332348b342053f917be832eab369bad2709e8

2020-09-24: wei.sun2
* dhcp6.c: Bug #1418548 (support Dhcpv6 infor relpy for ppp)
commit-ID:cafd5e393b728c6b31550f59009d1ea3f1a7b039

2020-09-24: xiwenmao
* upap.c: Fixed Bug #1404441 (support internal communication for NAT mode)
commit-ID: f5a8d13494fb205e92d29fc337ea85d4d56d13e4

2020-09-25: fei.pan
* api_lib.c,api_msg.c/.h: Fixed bug #1413764 (fix memory lack when call geraddrinfo)
commit-ID: d7b3a7915a9c11e436882140d639067dd58e0424

2020-10-29: fei.pan
* netif.c: Fixed bug #1439993 (callback_arg is null in sntp gethostbyname,it cause can not reslove IP,so sntp failed)
commit-ID: 8c13f32fb4097756e4fb6db72b63dd2fd72fbae3

2020-11-02: xiwen.mao
* udp.c,tcp.c/tcp.h: Fixed bug #1443676(change components/net traceID)
commit-ID: ad088c1aade8467c59a38d2ea1074b2038522e97

2020-11-05: xiwenmao
* icmp.c: Bug #1445139 (add ipv6 length check in ip6addr_aton)
commit-ID: 8e1340b08b50beed602d84bb137ced17b4543af1

2020-11-16: xiwen.mao
* tcp_in.c: Fixed bug #1454199(fix lwip out of order TCP data assert)
commit-ID: a299f7ac1463db0ccdb55d5d17ab57c5728e974b

2020-11-17: xiwenmao
* sockets.c: Fixed Bug #1454810 (fix assert when lwip_recvfrom mem is NULL)
commit-ID: c12552a75b0ffdfa6f7192742401806c817dccc7

2020-11-27: xiwen.mao
* api_msg.c,ip4_nat.c: Fixed bug #1462295 (add NAT out random port; fix lwip_close block when so_linger is set)
commit-ID: 339df48e9f9156217ecaaf328ab4a7e5228af6e5

2020-11-27: xiwenmao
* nd6.c:  Bug #1457478 (use CFW_NwGetFTA for GCF test)
commit-ID: 9d298557557e733385098192f859b74444bdc0a7

2020-12-04: xiwenmao
* netdb.c: Fixed Bug #1468356 (fix AT+PING crash before LWIP lib init)
commit-ID: 95266b5b3eff0af69aa396cdf670e40adf028807

2020-12-18: jialiu
* altcp_tls.c: Fixed  Bug #1275113 (fix double free when mqtts connection closed by peer)
commit-ID: 5474e454081f701057e990c1fa5190f704a45c61

2021-01-25: xiwenmao
* nd6.c:  Bug #1495932 (null pointer when bind lan to actived wan under nat)
commit-ID:8eb2c9e745715a4a9e50bf7d6135a4b1a33534a9

2021-02-01: xiwen.mao
* api_msg.c: Fixed bug #1505707 (fix double free in non-block so_linger)
commit-ID: 33362450ff510aad6c39c5873a2b31a3994337f5

2021-03-04: fei.pan
* dns.c: Fixed bug #1537797 (netif_default is null pointer,carsh)
commit-ID: ca9528b7f2651a60b38b6f3c6ccdf2ece90a73bb

2021-03-10: Xianyong.Cheng
* dns.c: Fixed bug #1500269 ( add multi dns support)
commit-ID: 53d819ce0fdc5a8bf20d4c6d43d0d268a8740134

2021-03-31: xiwenmao
* api_lib.c,sockets.c api.h...: Fixed bug #1531491 (add rfack in lwip; changed cfw data send api)
commit-ID: 1c161cb2616f324e433d91d57029b19dd54ce543

2021-04-08: Xianyong.Cheng
* dns.c: Fixed bug #1573026 (add nping and qdns at command support)
commit-ID: 8925ea557dbe351f2620623b9747a5b928b862c8

2021-04-09: xiwenmao
* api_lib.c,sockets.c api.h...: Fixed bug #1531491 (dissable rfack in 8811 CD atrgart)
commit-ID: 8c5b6999532cc29177b45c118cdcfb64444a2f96

2021-04-13: Xianyong.Cheng
* ip4_addr.c: Fixed Bug #1573026 (fix qdns not return OK issue)
commit-ID: 67b4801a7a41d1475e190dc876a3aa09a2fa6e00

2021-04-26: guangzu.sun
* api_msg.c: Fixed bug #1586944 (tcp connect no set so_linger para,close socket improper logic)
commit-ID: 7d598871482effa16d4b2eafb59803258f09b398

2021-05-18: guangzu.sun
* tcp_out.c,tcp.c: Fixed Bug #1597442 (Adds partial support for selective acknowledgements (RFC 2018).)
        Adds partial support for selective acknowledgements (RFC 2018).
        This change makes lwIP negotiate SACK support, and include SACK
        data in outgoing empty ACK packets. It does not include it
        in outgoing packets with data payload.
         It also does not add support for handling incoming SACKs.
commit-ID: 3421089ecbba39c6a31572bfad01ec4d25eb6dfa

2021-05-21: xiwen.mao
* api_msg.c,tcp.c/tcp.h: Fixed bug #1607709(fix so_linger close block error when socket not read all data)
commit-ID: b109b48f08e7abd478c2f91d46105e5bbe24c93f

2021-06-08: guangzu.sun
* tcp.c: Fixed Bug #1628330 (After waking up PM3,the timing of TCP_data is abnormal)
commit-ID: f1ff825ae48ddd7ed5532781e2fba7fa776f9acb

2021-07-19: howie.li
* udp.c,tcp.c/tcp.h: Fixed bug #1617967(Add the support for 8910 IDS)
commit-ID: 3187dd005a8eda9f85d490aa468ea5454c3274c7

2021-08-04: xiwenmao
* nd6.c:  Bug #1674075 (add 8850 LWIP init)
commit-ID:f7d17c0ea72a5a4979d8064c0368f0f2edf09fd9

2021-09-06: guangzu.sun
* tcp_in.c: Fixed bug #1701952 (TCP data transmission is interrupted.)
commit-ID: 3f0976dfd3eb5863f6cbf982fa094fcc1c581216

2021-09-07: wei.sun2
* netif.c: Fixed bug #1698373 ( support multi-lan for nat mode)
commit-ID: a2e44ef90331da5faa1597b46d67424534a045a8

2021-09-14: xiwen.mao
* ip4.c: Fixed  Bug #1698373 (remove IP_NAT_INTERNAL_FORWARD)
commit-ID:e8507e9e9edd0ec71e40e767a87f9f0cdc0a8114

2021-09-16: david.zhang1
* api_lib.c,dns.c/.h,opt.h,api_msh.h ip4_nat.h: Fixed bug #1627358 (Dns server support)
When RNDIS is actived, customer can use device as dns server.
               1. Receive user dns request, first find ipaddr in local cache(size:50).
               2. Build request and send to remote dns server if not hit in cache.
               3. Build dns answer package and send to user.
               4. Support IPV4 & IPV6.
               5. Only support domain -> ipaddr.
commit-ID: b22103207ccbff3afa09a776fc17ec43d2803f78








































