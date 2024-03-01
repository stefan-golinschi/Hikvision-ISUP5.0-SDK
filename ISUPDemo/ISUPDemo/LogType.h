#ifndef _EHOMEDEMO_LOGTYPE_H_
#define _EHOMEDEMO_LOGTYPE_H_

#ifndef _HC_NET_SDK_H_

/* 报警 */
//主类型
#define MAJOR_ALARM                        0x1
//次类型
#define MINOR_ALARM_IN                    0x1        /* 报警输入 */
#define MINOR_ALARM_OUT                    0x2        /* 报警输出 */
#define MINOR_MOTDET_START                0x3        /* 移动侦测报警开始 */
#define MINOR_MOTDET_STOP                0x4        /* 移动侦测报警结束 */
#define MINOR_HIDE_ALARM_START            0x5        /* 遮挡报警开始 */
#define MINOR_HIDE_ALARM_STOP            0x6        /* 遮挡报警结束 */
#define MINOR_VCA_ALARM_START            0x7        /*智能报警开始*/
#define MINOR_VCA_ALARM_STOP            0x8        /*智能报警停止*/
#define MINOR_ITS_ALARM_START           0x09    // 交通事件报警开始
#define MINOR_ITS_ALARM_STOP            0x0A    // 交通事件报警结束
//2010-11-10 网络报警日志
#define MINOR_NETALARM_START            0x0b    /*网络报警开始*/
#define MINOR_NETALARM_STOP             0x0c    /*网络报警结束*/
//2010-12-16 报警板日志，与"MINOR_ALARM_IN"配对使用
#define MINOR_NETALARM_RESUME            0x0d    /*网络报警恢复*/
//2012-4-5 IPC PIR、无线、呼救报警
#define MINOR_WIRELESS_ALARM_START      0x0e  /* 无线报警开始 */
#define MINOR_WIRELESS_ALARM_STOP          0x0f /* 无线报警结束 */
#define MINOR_PIR_ALARM_START           0x10  /* 人体感应报警开始 */
#define MINOR_PIR_ALARM_STOP               0x11  /* 人体感应报警结束 */
#define MINOR_CALLHELP_ALARM_START      0x12  /* 呼救报警开始 */
#define MINOR_CALLHELP_ALARM_STOP          0x13  /* 呼救报警结束 */
#define MINOR_IPCHANNEL_ALARMIN_START   0x14  //数字通道报警输入开始：PCNVR在接收到数字通道的MINOR_ALARM_IN产生“数字通道报警输入开始”，10s，再收不到MINOR_ALARM_IN，产生“数字通道报警输入结束”
#define MINOR_IPCHANNEL_ALARMIN_STOP    0x15  //数字通道报警输入开始：同上
#define MINOR_DETECTFACE_ALARM_START    0x16  /* 人脸侦测报警开始 */
#define MINOR_DETECTFACE_ALARM_STOP      0x17  /* 人脸侦测报警结束 */
#define MINOR_VQD_ALARM_START           0x18  //VQD报警
#define MINOR_VQD_ALARM_STOP            0x19  //VQD报警结束 
#define MINOR_VCA_SECNECHANGE_DETECTION 0x1a  //场景侦测报警 2013-07-16

#define MINOR_SMART_REGION_EXITING_BEGIN            0x1b  //离开区域侦测开始
#define MINOR_SMART_REGION_EXITING_END              0x1c  //离开区域侦测结束
#define MINOR_SMART_LOITERING_BEGIN                 0x1d  //徘徊侦测开始
#define MINOR_SMART_LOITERING_END                   0x1e  //徘徊侦测结束

#define MINOR_VCA_ALARM_LINE_DETECTION_BEGIN        0x20  
#define MINOR_VCA_ALARM_LINE_DETECTION_END          0x21  
#define MINOR_VCA_ALARM_INTRUDE_BEGIN                0x22  //区域侦测开始    
#define MINOR_VCA_ALARM_INTRUDE_END                       0x23  //区域侦测结束    
#define MINOR_VCA_ALARM_AUDIOINPUT                    0x24   //音频异常输入
#define MINOR_VCA_ALARM_AUDIOABNORMAL                 0x25   //声强突变
#define MINOR_VCA_DEFOCUS_DETECTION_BEGIN           0x26  //虚焦侦测开始
#define MINOR_VCA_DEFOCUS_DETECTION_END                0x27  //虚焦侦测结束

//民用NVR
#define MINOR_EXT_ALARM                             0x28/*IPC外部报警*/
#define MINOR_VCA_FACE_ALARM_BEGIN                    0x29    /*人脸侦测开始*/
#define MINOR_SMART_REGION_ENTRANCE_BEGIN           0x2a  //进入区域侦测开始
#define MINOR_SMART_REGION_ENTRANCE_END             0x2b  //进入区域侦测结束
#define MINOR_SMART_PEOPLE_GATHERING_BEGIN          0x2c  //人员聚集侦测开始
#define MINOR_SMART_PEOPLE_GATHERING_END            0x2d  //人员聚集侦测结束
#define MINOR_SMART_FAST_MOVING_BEGIN               0x2e  //快速运动侦测开始
#define MINOR_SMART_FAST_MOVING_END                 0x2f  //快速运动侦测结束

#define MINOR_VCA_FACE_ALARM_END                    0x30    /*人脸侦测结束*/
#define MINOR_VCA_SCENE_CHANGE_ALARM_BEGIN            0x31   /*场景变更侦测开始*/
#define MINOR_VCA_SCENE_CHANGE_ALARM_END            0x32   /*场景变更侦测结束*/
#define MINOR_VCA_ALARM_AUDIOINPUT_BEGIN            0x33   /*音频异常输入开始*/
#define MINOR_VCA_ALARM_AUDIOINPUT_END                0x34   /*音频异常输入结束*/
#define MINOR_VCA_ALARM_AUDIOABNORMAL_BEGIN            0x35  /*声强突变侦测开始*/
#define MINOR_VCA_ALARM_AUDIOABNORMAL_END            0x36  /*声强突变侦测结束*/

#define MINOR_VCA_LECTURE_DETECTION_BEGIN           0x37  //授课侦测开始报警
#define MINOR_VCA_LECTURE_DETECTION_END             0x38  //授课侦测结束报警
#define MINOR_VCA_ALARM_AUDIOSTEEPDROP              0x39  //声强陡降 2014-03-21
#define MINOR_VCA_ANSWER_DETECTION_BEGIN            0x3a  //回答问题侦测开始报警
#define MINOR_VCA_ANSWER_DETECTION_END              0x3b  //回答问题侦测结束报警

#define MINOR_SMART_PARKING_BEGIN                   0x3c   //停车侦测开始
#define MINOR_SMART_PARKING_END                     0x3d   //停车侦测结束
#define MINOR_SMART_UNATTENDED_BAGGAGE_BEGIN        0x3e   //物品遗留侦测开始
#define MINOR_SMART_UNATTENDED_BAGGAGE_END          0x3f   //物品遗留侦测结束
#define MINOR_SMART_OBJECT_REMOVAL_BEGIN            0x40   //物品拿取侦测开始
#define MINOR_SMART_OBJECT_REMOVAL_END              0x41   //物品拿取侦测结束
#define MINOR_SMART_VEHICLE_ALARM_START             0x46   //车牌检测开始
#define MINOR_SMART_VEHICLE_ALARM_STOP              0x47   //车牌检测结束
#define MINOR_THERMAL_FIREDETECTION                 0x48   //热成像火点检测侦测开始
#define MINOR_THERMAL_FIREDETECTION_END             0x49   //热成像火点检测侦测结束
#define MINOR_SMART_VANDALPROOF_BEGIN               0x50   //防破坏检测开始
#define MINOR_SMART_VANDALPROOF_END                 0x51   //防破坏检测结束

#define MINOR_FACESNAP_MATCH_ALARM_START            0x55  /*人脸比对报警开始*/
#define MINOR_FACESNAP_MATCH_ALARM_STOP             0x56  /*人脸比对报警结束*/
#define MINOR_WHITELIST_FACESNAP_MATCH_ALARM_START  0x57  /*白名单人脸比对（陌生人）报警开始*/
#define MINOR_WHITELIST_FACESNAP_MATCH_ALARM_STOP   0x58  /*白名单人脸比对（陌生人）报警结束*/

#define MINOR_THERMAL_SHIPSDETECTION                0x5a   //热成像船只检测侦测
#define MINOR_THERMAL_THERMOMETRY_EARLYWARNING_BEGIN 0x5b  //热成像测温预警开始
#define MINOR_THERMAL_THERMOMETRY_EARLYWARNING_END  0x5c   //热成像测温预警结束
#define MINOR_THERMAL_THERMOMETRY_ALARM_BEGIN       0x5d   //热成像测温报警开始
#define MINOR_THERMAL_THERMOMETRY_ALARM_END         0x5e   //热成像测温报警结束
#define MINOR_THERMAL_THERMOMETRY_DIFF_ALARM_BEGIN  0x5f   //热成像温差报警开始
#define MINOR_THERMAL_THERMOMETRY_DIFF_ALARM_END    0x60   //热成像温差报警结束
#define MINOR_FACE_THERMOMETRY_ALARM                0x63   //人脸测温报警
#define MINOR_SMART_DENSEFOGDETECTION_BEGIN         0x6e   //大雾侦测开始
#define MINOR_SMART_DENSEFOGDETECTION_END           0x6f   //大雾侦测结束
#define MINOR_RUNNING_ALARM                         0x70   //奔跑检测
#define MINOR_RETENTION_ALARM                       0x71   //滞留检测
#define MINOR_SAFETY_HELMET_ALARM_START             0x72  /*未佩戴安全帽检测报警开始*/
#define MINOR_SAFETY_HELMET_ALARM_STOP              0x73  /*未佩戴安全帽检测报警结束*/
#define MINOR_HFPD_ALARM_START                      0x74  /*高频人员检测报警开始*/
#define MINOR_HFPD_ALARM_STOP                       0x75  /*高频人员检测报警结束*/
#define MINOR_MIXED_TARGET_ALARM_START              0x76  /*混合目标检测报警开始*/
#define MINOR_MIXED_TARGET_ALARM_STOP               0x77  /*混合目标检测报警结束*/
#define MINOR_VCA_GET_UP_ALARM_BEGIN                0x80   //起床检测报警开始
#define MINOR_VCA_GET_UP_ALARM_END                  0x81   //起床检测报警结束
#define MINOR_VCA_ADV_REACH_HEIGHT_ALARM_BEGIN      0x82   //折线攀高报警开始
#define MINOR_VCA_ADV_REACH_HEIGHT_ALARM_END        0x83   //折线攀高报警结束
#define MINOR_VCA_TOILET_TARRY_ALARM_BEGIN          0x84   //如厕超时报警开始
#define MINOR_VCA_TOILET_TARRY_ALARM_END            0x85   //如厕超时报警结束
#define MINOR_HUMAN_RECOGNITION_ALARM_BEGIN         0x86   //人体识别报警开始
#define MINOR_HUMAN_RECOGNITION_ALARM_END           0x87   //人体识别报警结束
#define MINOR_STUDENTS_STOODUP_ALARM_BEGIN          0x88   //学生起立报警开始
#define MINOR_STUDENTS_STOODUP_ALARM_END            0x89   //学生起立报警结束
#define MINOR_FRAMES_PEOPLE_COUNTING_ALARM          0x8a   //区域人数统计报警
#define MINOR_FACE_SNAP_ALARM_BEGIN                 0x8b   //人脸抓拍报警开始
#define MINOR_FACE_SNAP_ALARM_END                   0x8c   //人脸抓拍报警结束
#define MINOR_TEACHER_BEHAVIOR_DETECT_ALARM_BEGIN   0x8d   //教师行为检测报警开始
#define MINOR_TEACHER_BEHAVIOR_DETECT_ALARM_END     0x8e   //教师行为检测报警结束
#define MINOR_PERIMETER_CAPTURE_ALARM_BEGIN         0x8f   //周界抓拍报警开始
#define MINOR_PERIMETER_CAPTURE_ALARM_END           0x90   //周界抓拍报警结束
#define MINOR_UNREGISTERED_STREET_VENDOR_ALARM      0x91  //非法摆摊报警

#define MINOR_PERSON_QUEUE_TIME_ALARM_BEGIN         0x92   //排队时长检测报警开始
#define MINOR_PERSON_QUEUE_TIME_ALARM_END           0x93   //排队时长检测报警结束
#define MINOR_PERSON_QUEUE_COUNTING_ALARM_BEGIN     0x94   //排队人数检测报警开始
#define MINOR_PERSON_QUEUE_COUNTING_ALARM_END       0x95   //排队人数检测报警结束


#define MINOR_ACCESS_CONTROLLER_EVENT               0x100  //门禁主机事件
#define MINOR_VIDEO_INTERCOM_EVENT                  0x101  //可视对讲设备事件
#define MINOR_GJD_EVENT                             0x102  //GJD报警主机事件
#define MINOR_LUMINITE_EVENT                        0x103  // LUMINITE报警主机事件
#define MINOR_OPTEX_EVENT                           0x104  // OPTEX报警主机事件
#define MINOR_CAMERA_DETECTOR_EVENT                 0x105  // 传感器事件
#define MINOR_SECURITY_CONTROL_PANEL_EVENT          0x106  //海康报警主机事件

#define MINOR_VCA_SPACE_CHANGE_START                0x10c //间距异常检测开始
#define MINOR_VCA_SPACE_CHANGE_STOP                 0x10d //间距异常检测结束
#define MINOR_MANUAL_ALARM                          0x10e //手动报警
#define MINOR_DETECTOR_ALARM                        0x10f //探测器报警
#define MINOR_LINKAGE_ALARM                         0x110 //联动报警
#define MINOR_VCA_SITUATION_ANALYSIS_START          0x111 //态势分析检测开始
#define MINOR_VCA_SITUATION_ANALYSIS_STOP           0x112 //态势分析检测结束

//0x400-0x1000 门禁报警
#define MINOR_ALARMIN_SHORT_CIRCUIT                 0x400  //防区短路报警
#define MINOR_ALARMIN_BROKEN_CIRCUIT                0x401  //防区断路报警
#define MINOR_ALARMIN_EXCEPTION                     0x402  //防区异常报警
#define MINOR_ALARMIN_RESUME                        0x403  //防区报警恢复
#define MINOR_HOST_DESMANTLE_ALARM                  0x404  //设备防拆报警
#define MINOR_HOST_DESMANTLE_RESUME                 0x405  //设备防拆恢复
#define MINOR_CARD_READER_DESMANTLE_ALARM           0x406  //读卡器防拆报警
#define MINOR_CARD_READER_DESMANTLE_RESUME          0x407  //读卡器防拆恢复
#define MINOR_CASE_SENSOR_ALARM                     0x408  //事件输入报警
#define MINOR_CASE_SENSOR_RESUME                    0x409  //事件输入恢复
#define MINOR_STRESS_ALARM                          0x40a  //胁迫报警
#define MINOR_OFFLINE_ECENT_NEARLY_FULL             0x40b  //离线事件满90%报警
#define MINOR_CARD_MAX_AUTHENTICATE_FAIL            0x40c  //卡号认证失败超次报警
#define MINOR_SD_CARD_FULL                          0x40d  //SD卡存储满报警
#define MINOR_LINKAGE_CAPTURE_PIC                   0x40e  //联动抓拍事件报警
#define MINOR_SECURITY_MODULE_DESMANTLE_ALARM        0x40f  //门控安全模块防拆报警
#define MINOR_SECURITY_MODULE_DESMANTLE_RESUME        0x410  //门控安全模块防拆恢复

#define MINOR_POS_START_ALARM                       0x411  //POS开启
#define MINOR_POS_END_ALARM                         0x412  //POS结束
#define MINOR_FACE_IMAGE_QUALITY_LOW                0x413  //人脸图像画质低
#define MINOR_FINGE_RPRINT_QUALITY_LOW              0x414  //指纹图像画质低
#define MINOR_FIRE_IMPORT_SHORT_CIRCUIT             0x415  //消防输入短路报警
#define MINOR_FIRE_IMPORT_BROKEN_CIRCUIT            0x416  //消防输入断路报警
#define MINOR_FIRE_IMPORT_RESUME                    0x417  //消防输入恢复
#define MINOR_FIRE_BUTTON_TRIGGER                   0x418  //消防按钮触发
#define MINOR_FIRE_BUTTON_RESUME                    0x419  //消防按钮恢复
#define MINOR_MAINTENANCE_BUTTON_TRIGGER            0x41a  //维护按钮触发
#define MINOR_MAINTENANCE_BUTTON_RESUME             0x41b  //维护按钮恢复
#define MINOR_EMERGENCY_BUTTON_TRIGGER              0x41c  //紧急按钮触发
#define MINOR_EMERGENCY_BUTTON_RESUME               0x41d  //紧急按钮恢复
#define MINOR_DISTRACT_CONTROLLER_ALARM             0x41e  //分控器防拆报警
#define MINOR_DISTRACT_CONTROLLER_RESUME            0x41f  //分控器防拆报警恢复

#define MINOR_PERSON_DENSITY_DETECTION_START        0x420  //人员密度超阈值报警开始
#define MINOR_PERSON_DENSITY_DETECTION_END          0x421  //人员密度超阈值报警结束

#define MINOR_CHANNEL_CONTROLLER_DESMANTLE_ALARM    0x422  //通道控制器防拆报警
#define MINOR_CHANNEL_CONTROLLER_DESMANTLE_RESUME   0x423  //通道控制器防拆报警恢复
#define MINOR_CHANNEL_CONTROLLER_FIRE_IMPORT_ALARM  0x424  //通道控制器消防输入报警
#define MINOR_CHANNEL_CONTROLLER_FIRE_IMPORT_RESUME 0x425  //通道控制器消防输入报警恢复

#define MINOR_HEART_RATE_ABNORMAL_BEGIN             0x426    //心率异常报警开始
#define MINOR_HEART_RATE_ABNORMAL_END               0x427    //心率异常报警结束
#define MINOR_BLOOD_OXYGEN_ABNORMAL_BEGIN           0x428    //血氧异常报警开始
#define MINOR_BLOOD_OXYGEN_ABNORMAL_END             0x429    //血氧异常报警结束
#define MINOR_SYSTOLIC_BLOOD_PRESSURE_ABNORMAL_BEGIN   0x42a    //血压收缩压异常报警开始
#define MINOR_SYSTOLIC_BLOOD_PRESSURE_ABNORMAL_END     0x42b    //血压收缩压异常报警结束
#define MINOR_DIASTOLIC_BLOOD_PRESSURE_ABNORMAL_BEGIN  0x42c    //血压舒张压异常报警开始
#define MINOR_DIASTOLIC_BLOOD_PRESSURE_ABNORMAL_END    0x42d    //血压舒张压异常报警结束
#define MINOR_VCA_LEAVE_POSITION_START              0x42e//离岗检测开始
#define MINOR_VCA_LEAVE_POSITION_STOP               0x42f//离岗检测结束
#define MINOR_VCA_STOOODUP_START                    0x430//起立检测开始
#define MINOR_VCA_STOOODUP_STOP                     0x431//起立检测结束
#define MINOR_VCA_PEOPLENUM_CHANGE_START            0x434//人数变化开始
#define MINOR_VCA_PEOPLENUM_CHANGE_STOP             0x435//人数变化结束
#define MINOR_VCA_RUNNING_START                     0x438//人员奔跑开始
#define MINOR_VCA_RUNNING_STOP                      0x439//人员奔跑结束
#define MINOR_VCA_VIOLENT_MOTION_START              0x43a//剧烈运动开始
#define MINOR_VCA_VIOLENT_MOTION_STOP               0x43b//剧烈运动结束
#define MINOR_VCA_FAIL_DOWN_START                   0x43c//人员倒地开始
#define MINOR_VCA_FAIL_DOWN_STOP                    0x43d//人员倒地结束
#define MINOR_VCA_RETENTION_START                   0x43e//人员滞留开始
#define MINOR_VCA_RETENTION_STOP                    0x43f//人员滞留结束

#define MINOR_PRINTER_OUT_OF_PAPER               0x440   //打印机缺纸报警
#define MINOR_LEGAL_EVENT_NEARLY_FULL            0x442   //离线合法事件满90%报警

#define MINOR_ALARM_CUSTOM1                         0x900  //门禁自定义报警1
#define MINOR_ALARM_CUSTOM2                         0x901  //门禁自定义报警2
#define MINOR_ALARM_CUSTOM3                         0x902  //门禁自定义报警3
#define MINOR_ALARM_CUSTOM4                         0x903  //门禁自定义报警4
#define MINOR_ALARM_CUSTOM5                         0x904  //门禁自定义报警5
#define MINOR_ALARM_CUSTOM6                         0x905  //门禁自定义报警6
#define MINOR_ALARM_CUSTOM7                         0x906  //门禁自定义报警7
#define MINOR_ALARM_CUSTOM8                         0x907  //门禁自定义报警8
#define MINOR_ALARM_CUSTOM9                         0x908  //门禁自定义报警9
#define MINOR_ALARM_CUSTOM10                        0x909  //门禁自定义报警10
#define MINOR_ALARM_CUSTOM11                        0x90a  //门禁自定义报警11
#define MINOR_ALARM_CUSTOM12                        0x90b  //门禁自定义报警12
#define MINOR_ALARM_CUSTOM13                        0x90c  //门禁自定义报警13
#define MINOR_ALARM_CUSTOM14                        0x90d  //门禁自定义报警14
#define MINOR_ALARM_CUSTOM15                        0x90e  //门禁自定义报警15
#define MINOR_ALARM_CUSTOM16                        0x90f  //门禁自定义报警16
#define MINOR_ALARM_CUSTOM17                        0x910  //门禁自定义报警17
#define MINOR_ALARM_CUSTOM18                        0x911  //门禁自定义报警18
#define MINOR_ALARM_CUSTOM19                        0x912  //门禁自定义报警19
#define MINOR_ALARM_CUSTOM20                        0x913  //门禁自定义报警20
#define MINOR_ALARM_CUSTOM21                        0x914  //门禁自定义报警21
#define MINOR_ALARM_CUSTOM22                        0x915  //门禁自定义报警22
#define MINOR_ALARM_CUSTOM23                        0x916  //门禁自定义报警23
#define MINOR_ALARM_CUSTOM24                        0x917  //门禁自定义报警24
#define MINOR_ALARM_CUSTOM25                        0x918  //门禁自定义报警25
#define MINOR_ALARM_CUSTOM26                        0x919  //门禁自定义报警26
#define MINOR_ALARM_CUSTOM27                        0x91a  //门禁自定义报警27
#define MINOR_ALARM_CUSTOM28                        0x91b  //门禁自定义报警28
#define MINOR_ALARM_CUSTOM29                        0x91c  //门禁自定义报警29
#define MINOR_ALARM_CUSTOM30                        0x91d  //门禁自定义报警30
#define MINOR_ALARM_CUSTOM31                        0x91e  //门禁自定义报警31
#define MINOR_ALARM_CUSTOM32                        0x91f  //门禁自定义报警32
#define MINOR_ALARM_CUSTOM33                        0x920  //门禁自定义报警33
#define MINOR_ALARM_CUSTOM34                        0x921  //门禁自定义报警34
#define MINOR_ALARM_CUSTOM35                        0x922  //门禁自定义报警35
#define MINOR_ALARM_CUSTOM36                        0x923  //门禁自定义报警36
#define MINOR_ALARM_CUSTOM37                        0x924  //门禁自定义报警37
#define MINOR_ALARM_CUSTOM38                        0x925  //门禁自定义报警38
#define MINOR_ALARM_CUSTOM39                        0x926  //门禁自定义报警39
#define MINOR_ALARM_CUSTOM40                        0x927  //门禁自定义报警40
#define MINOR_ALARM_CUSTOM41                        0x928  //门禁自定义报警41
#define MINOR_ALARM_CUSTOM42                        0x929  //门禁自定义报警42
#define MINOR_ALARM_CUSTOM43                        0x92a  //门禁自定义报警43
#define MINOR_ALARM_CUSTOM44                        0x92b  //门禁自定义报警44
#define MINOR_ALARM_CUSTOM45                        0x92c  //门禁自定义报警45
#define MINOR_ALARM_CUSTOM46                        0x92d  //门禁自定义报警46
#define MINOR_ALARM_CUSTOM47                        0x92e  //门禁自定义报警47
#define MINOR_ALARM_CUSTOM48                        0x92f  //门禁自定义报警48
#define MINOR_ALARM_CUSTOM49                        0x930  //门禁自定义报警49
#define MINOR_ALARM_CUSTOM50                        0x931  //门禁自定义报警50
#define MINOR_ALARM_CUSTOM51                        0x932  //门禁自定义报警51
#define MINOR_ALARM_CUSTOM52                        0x933  //门禁自定义报警52
#define MINOR_ALARM_CUSTOM53                        0x934  //门禁自定义报警53
#define MINOR_ALARM_CUSTOM54                        0x935  //门禁自定义报警54
#define MINOR_ALARM_CUSTOM55                        0x936  //门禁自定义报警55
#define MINOR_ALARM_CUSTOM56                        0x937  //门禁自定义报警56
#define MINOR_ALARM_CUSTOM57                        0x938  //门禁自定义报警57
#define MINOR_ALARM_CUSTOM58                        0x939  //门禁自定义报警58
#define MINOR_ALARM_CUSTOM59                        0x93a  //门禁自定义报警59
#define MINOR_ALARM_CUSTOM60                        0x93b  //门禁自定义报警60
#define MINOR_ALARM_CUSTOM61                        0x93c  //门禁自定义报警61
#define MINOR_ALARM_CUSTOM62                        0x93d  //门禁自定义报警62
#define MINOR_ALARM_CUSTOM63                        0x93e  //门禁自定义报警63
#define MINOR_ALARM_CUSTOM64                        0x93f  //门禁自定义报警64
#define MINOR_LOCK_HIJIACK_FINGER_ALARM         0x950       //智能锁防劫持指纹报警
#define MINOR_LOCK_HIJIACK_PASSWORD_ALARM       0x951       //智能锁防劫持密码报警
#define MINOR_LOCK_PRY_DOOR_ALARM               0x952       //智能锁撬门报警
#define MINOR_LOCK_LOCKED_ALARM                 0x953       //智能锁锁定报警
#define MINOR_LOCK_BATTERLOW_ALARM              0x954       //智能锁低电压报警
#define MINOR_LOCK_BLACKLIST_DOOR_ALARM         0x955       //智能锁黑名单报警
#define MINOR_LOCK_OFFLINE_ALARM                0x956       //智能锁离线报警
#define MINOR_LOCK_UNCLOSED_ALARM               0x957       //智能锁虚掩报警
#define MINOR_LOCK_NO_HOME_ALARM                0x958       //智能锁用户未回家报警
#define MINOR_LOCK_MAGNETOMETER_ALARM           0x959       //门磁探测器报警
#define MINOR_LOCK_IR_DETECTOR_ALARM            0x95a       //红外探测器报警
#define MINOR_LOCK_FP_LOCKED_ALARM              0x95b       //指纹锁定报警
#define MINOR_LOCK_PASSWORD_LOCKED_ALARM        0x95c       //密码锁定报警
#define MINOR_LOCK_HIJIACK_ALARM              0x95d  //智能锁防劫持报警

//2018-04-23 通用物联网关报警日志类型
#define MINOR_ALARMHOST_SHORT_CIRCUIT    	      0x1001      //短路报警
#define MINOR_ALARMHOST_BROKEN_CIRCUIT   		  0x1002       //断路报警
#define MINOR_ALARMHOST_ALARM_RESET      	      0x1003       //报警复位
#define MINOR_ALARMHOST_ALARM_NORMAL			  0x1004   	//报警恢复正常
#define MINOR_ALARMHOST_PASSWORD_ERROR 		  0x1005		//密码错误（连续3次输入密码错误）
#define MINOR_ALARMHOST_ID_CARD_ILLEGALLY 	  0x1006		//非法感应卡ID
#define MINOR_ALARMHOST_KEYPAD_REMOVE 		  0x1007	//键盘防拆
#define MINOR_ALARMHOST_KEYPAD_REMOVE_RESTORE   0x1008	//键盘防拆复位

#define MINOR_ALARMHOST_BELOW_ALARM_LIMIT1 	  0x1011	//模拟量低于报警限1
#define MINOR_ALARMHOST_BELOW_ALARM_LIMIT2 	  0x1012    //模拟量低于报警限2
#define MINOR_ALARMHOST_BELOW_ALARM_LIMIT3 	  0x1013    //模拟量低于报警限3
#define MINOR_ALARMHOST_BELOW_ALARM_LIMIT4 	  0x1014    //模拟量低于报警限4
#define MINOR_ALARMHOST_ABOVE_ALARM_LIMIT1 	  0x1015    //模拟量高于报警限1
#define MINOR_ALARMHOST_ABOVE_ALARM_LIMIT2 	0x1016    //模拟量高于报警限2
#define MINOR_ALARMHOST_ABOVE_ALARM_LIMIT3 	0x1017     //模拟量高于报警限3
#define MINOR_ALARMHOST_ABOVE_ALARM_LIMIT4 	0x1018     //模拟量高于报警限4

#define MINOR_ALARMHOST_VIRTUAL_DEFENCE_BANDIT 	0x1021      //软防区匪警
#define MINOR_ALARMHOST_VIRTUAL_DEFENCE_FIRE 		0x1022       //软防区火警
#define MINOR_ALARMHOST_VIRTUAL_DEFENCE_URGENT 		0x1023       //软防区紧急

#define MINOR_UPS_ALARM			0x1028      //UPS报警
#define MINOR_ELECTRICITY_METER_ALARM 0x1029     //智能电表报警
#define MINOR_SWITCH_POWER_ALARM	  0x1030     //开关电源报警
#define MINOR_GAS_DETECT_SYS_ALARM	  0x1031    //气体检测系统报警
#define MINOR_TRANSFORMER_TEMPRATURE_ALARM 0x1032     //变电器温显表报警
#define MINOR_TEMP_HUMI_ALARM		0x1033   //温湿度传感器报警
#define MINOR_UPS_ALARM_RESTORE	0x1034    //UPS报警恢复
#define MINOR_ELECTRICITY_METER_ALARM_RESTORE 0x1035   //智能电表报警恢复
#define MINOR_SWITCH_POWER_ALARM_RESTORE	0x1036   //开关电源报警恢复
#define MINOR_GAS_DETECT_SYS_ALARM_RESTORE	  0x1037   //气体检测系统报警恢复
#define MINOR_TRANSFORMER_TEMPRATURE_ALARM_RESTORE 0x1038            //变电器温显表报警恢复
#define MINOR_TEMP_HUMI_ALARM_RESTORE	0x1039     //温湿度传感器报警恢复
#define MINOR_WATER_LEVEL_SENSOR_ALARM 		0x1040     //水位传感器报警
#define MINOR_WATER_LEVEL_SENSOR_ALARM_RESTORE 	0x1041  //水位传感器报警恢复
#define MINOR_DUST_NOISE_ALARM 	0x1042  //扬尘噪声传感器报警
#define MINOR_DUST_NOISE_ALARM_RESTORE 	0x1043     //扬尘噪声传感器报警恢复
#define MINOR_ENVIRONMENTAL_LOGGER_ALARM 	0x1044    //环境采集仪报警
#define MINOR_ENVIRONMENTAL_LOGGER_ALARM_RESTORE 0x1045            //环境采集仪报警恢复

#define MINOR_TRIGGER_TAMPER    0x1046     //探测器防拆
#define MINOR_TRIGGER_TAMPER_RESTORE     0x1047    //探测器防拆恢复
#define MINOR_EMERGENCY_CALL_HELP_ALARM    0x1048       //紧急呼叫求助报警
#define MINOR_EMERGENCY_CALL_HELP_ALARM_RESTORE 0x1049            //紧急呼叫求助报警恢复
#define MINOR_CONSULTING_ALARM     0x1050    //业务咨询报警
#define MINOR_CONSULTING_ALARM_RESTORE   0x1051     //业务咨询报警恢复
#define MINOR_ALARMHOST_ZONE_MODULE_REMOVE      0x1052    //防区模块防拆
#define MINOR_ALARMHOST_ZONE_MODULE_RESET     0x1053       //防区模块防拆复位

#define MINOR_ALARMHOST_ALARM_WIND_SPEED_ALARM      0x1054      //风速传感器告警
#define MINOR_ALARMHOST_ALARM_WIND_SPEED_ALARM_RESTORE   0x1055      //风速传感器告警恢复
#define MINOR_ALARMHOST_ALARM_GENERATE_OUTPUT_ALARM   0x1056   //通用扩展输出模块告警
#define MINOR_ALARMHOST_ALARM_GENERATE_OUTPUT_RESTORE    0x1057     //通用扩展输出模块告警恢复
#define MINOR_ALARMHOST_ALARM_SOAK_ALARM      0x1058      //水浸传感器告警
#define MINOR_ALARMHOST_ALARM_SOAK_ALARM_RESTORE   0x1059  //水浸传感器告警恢复
#define MINOR_ALARMHOST_ALARM_SOLAR_POWER_ALARM    0x1060    //太阳能传感器告警
#define MINOR_ALARMHOST_ALARM_SOLAR_POWER_ALARM_RESTORE 0x1061   //太阳能传感器告警恢复
#define MINOR_ALARMHOST_ALARM_SF6_ALARM   0x1062      //SF6报警主机告警
#define MINOR_ALARMHOST_ALARM_SF6_ALARM_RESTORE   0x1063    //SF6报警主机告警恢复
#define MINOR_ALARMHOST_ALARM_WEIGHT_ALARM    0x1064      //称重仪告警
#define MINOR_ALARMHOST_ALARM_WEIGHT_ALARM_RESTORE    0x1065   //称重仪告警恢复
#define MINOR_ALARMHOST_ALARM_WEATHER_ALARM   0x1066   //气象采集系统告警
#define MINOR_ALARMHOST_ALARM_WEATHER_ALARM_RESTORE    0x1067     //气象采集系统告警恢复
#define MINOR_ALARMHOST_ALARM_FUEL_GAS_ALARM     0x1068  //燃气监测系统告警
#define MINOR_ALARMHOST_ALARM_FUEL_GAS_ALARM_RESTORE  0x1069 //燃气监测系统告警恢
#define MINOR_ALARMHOST_ALARM_FIRE_ALARM   0x1070  //火灾报警系统告警
#define MINOR_ALARMHOST_ALARM_FIRE_ALARM_RESTORE    0x1071   //火灾报警系统告警恢复
#define  MINOR_ALARMHOST_WIRELESS_OUTPUT_MODULE_REMOVE     0x1072   //无线输出模块防拆
#define  MINOR_ALARMHOST_WIRELESS_OUTPUT_MODULE_RESET    0x1073  //无线输出模块防拆复位
#define  MINOR_ALARMHOST_WIRELESS_REPEATER_MODULE_REMOVE   0x1074  //无线中继器防拆
#define  MINOR_ALARMHOST_WIRELESS_REPEATER_MODULE_RESET  0x1074 //无线中继器防拆复位
#define  MINOR_ALARMHOST_WIRELESS_SIREN_MODULE_REMOVE     0x1075  //无线警号防拆
#define  MINOR_ALARMHOST_WIRELESS_SIREN_MODULE_RESET     0x1076 //无线警号防拆复位

#define  MINOR_RS485_DEV_ALARM     0x1077      //RS485外接设备报警（针对设备类型未知的设备）
#define  MINOR_RS485_DEV_RESTORE    0x1078     //RS485外接设备报警恢复（针对设备类型未知的设备）
#define  MINOR_ALARMHOST_ALARM_HOST_ALARM     0x1079      //消防主机报警
#define  MINOR_ALARMHOST_ALARM_HOST_RESTORE    0x107a     //消防主机报警恢复

#define  MINOR_AIR_CONDITION_DEV_ALARM     0x107b      //空调控制器报警
#define  MINOR_AIR_CONDITION_DEV_RESTORE    0x107c     //空调控制器报警恢复


/* 异常 */
//主类型
#define MAJOR_EXCEPTION                    0x2
//次类型
#define MINOR_SUBSYSTEM_ERROR           0x0a    /* 子系统异常 */
#define MINOR_RAID_ERROR                0x20    /* 阵列异常 */
#define MINOR_VI_LOST                    0x21    /* 视频信号丢失 */
#define MINOR_ILLEGAL_ACCESS            0x22    /* 非法访问 */
#define MINOR_HD_FULL                    0x23    /* 硬盘满 */
#define MINOR_HD_ERROR                    0x24    /* 硬盘错误 */
#define MINOR_DCD_LOST                    0x25    /* MODEM 掉线(保留不使用) */
#define MINOR_IP_CONFLICT                0x26    /* IP地址冲突 */
#define MINOR_NET_BROKEN                0x27    /* 网络断开*/
#define MINOR_REC_ERROR                 0x28    /* 录像出错 */
#define MINOR_IPC_NO_LINK               0x29    /* IPC连接异常 */
#define MINOR_VI_EXCEPTION              0x2a    /* 视频输入异常(只针对模拟通道) */
#define MINOR_IPC_IP_CONFLICT           0x2b    /*ipc ip 地址 冲突*/
#define MINOR_SENCE_EXCEPTION           0x2c    // 场景异常

#define MINOR_PIC_REC_ERROR              0x2d    /* 抓图出错--获取图片文件失败*/
#define MINOR_VI_MISMATCH                0x2e    /* 视频制式不匹配*/
#define MINOR_RESOLUTION_MISMATCH        0x2f    /*前端/录像分辨率不匹配  */

//2009-12-16 增加视频综合平台日志类型
#define MINOR_FANABNORMAL                0x31    /* 视频综合平台：风扇状态异常 */
#define MINOR_FANRESUME                    0x32    /* 视频综合平台：风扇状态恢复正常 */
#define MINOR_SUBSYSTEM_ABNORMALREBOOT    0x33    /* 视频综合平台：6467异常重启 */
#define MINOR_MATRIX_STARTBUZZER        0x34    /* 视频综合平台：dm6467异常，启动蜂鸣器 */

//2010-01-22 增加视频综合平台异常日志次类型
#define MINOR_NET_ABNORMAL                0x35    /*网络状态异常*/
#define MINOR_MEM_ABNORMAL                0x36    /*内存状态异常*/
#define MINOR_FILE_ABNORMAL                0x37    /*文件状态异常*/
#define MINOR_PANEL_ABNORMAL            0x38 /*前面板连接异常*/
#define MINOR_PANEL_RESUME                0x39 /*前面板恢复正常*/    
#define MINOR_RS485_DEVICE_ABNORMAL        0x3a     /*RS485连接状态异常*/
#define MINOR_RS485_DEVICE_REVERT        0x3b    /*RS485连接状态异常恢复*/

//2012-2-18 增加大屏控制器异常日志次类型
#define MINOR_SCREEN_SUBSYSTEM_ABNORMALREBOOT   0x3c            //子板异常启动
#define    MINOR_SCREEN_SUBSYSTEM_ABNORMALINSERT    0x3d            //子板插入
#define    MINOR_SCREEN_SUBSYSTEM_ABNORMALPULLOUT    0x3e            //子板拔出
#define    MINOR_SCREEN_ABNARMALTEMPERATURE        0x3f            //温度异常
//2012-07-26 视频综合平台v2.1
#define MINOR_HIGH_TEMPERATURE_PROTECT          0x40 //子板过热保护

//Netra 2.2.2
#define MINOR_RECORD_OVERFLOW                   0x41              /*缓冲区溢出*/  
#define MINOR_DSP_ABNORMAL                      0x42              //DSP异常 

//Netra 3.0.0
#define MINOR_ANR_RECORD_FAIED                 0x43         /*ANR录像失败*/ 
#define MINOR_SPARE_WORK_DEVICE_EXCEPT         0x44            /*热备设备工作机异常*/
#define MINOR_START_IPC_MAS_FAILED             0x45            /*开启IPC MAS失败*/ 
//高性能 256路NVR
#define MINOR_IPCM_CRASH                       0x46         /*IPCM异常重启*/
#define MINOR_POE_POWER_EXCEPTION              0x47         /*POE 供电异常*/
#define MINOR_UPLOAD_DATA_CS_EXCEPTION         0x48          //云存储数据上传失败/
#define MINOR_DIAL_EXCEPTION                   0x49         /*拨号异常*/
#define MINOR_DEV_EXCEPTION_OFFLINE            0x50  //设备异常下线
#define MINOR_UPGRADEFAIL                      0x51 //远程升级设备失败
#define MINOR_AI_LOST                           0x52    /* 音频信号丢失 */
#define MINOR_SYNC_IPC_PASSWD                   0x53    /* 同步IPC密码异常 */
#define MINOR_EZVIZ_OFFLINE                        0x54    /* 萤石下线异常*/
#define MINOR_VQD_ABNORMAL                        0x55 //VQD异常
#define MINOR_ACCESSORIES_PLATE                0x57 //配件板异常
#define MINOR_KMS_EXPAMSION_DISK_LOST         0x58 // KMS扩容盘丢失
#define MINOR_ABNORMAL_PORT                 0x59 // 端口异常
#define MINOR_CAMERA_ANGLE_ANOMALY          0x60//  相机视角异常
#define MINOR_DATA_DISK_ERROE               0x61//  数据盘错误  
#define MINOR_INTELLIGENT_SYSTEM_RUNNING_ERROR     0x62//  智能系统运行异常
#define MINOR_FACESNAP_RESOLUTION_OVERFLOW         0x63//  人脸抓拍码流分辨率超限
#define MINOR_SMD_RESOLUTION_OVERFLOW              0x64//  SMD码流分辨率超限
#define MINOR_AUDIO_LOSS_EXCEPTION                 0x65//  音频丢失异常
#define MINOR_SAFETY_HELMET_EXCEPTION              0x66 //未佩戴安全帽检测异常
#define MINOR_VCA_PIC_LENGTH_OVERFLOW              0x67 // VCA图片长度过长（例如超过2M大小的图片）
#define MINOR_FACE_MODEL_EXCEPTION                 0x68//  人脸库模型同步异常
//NVR集群
#define MINOR_CLUSTER_DEVICE_OFFLINE                 0x70 // 集群内设备下线
#define MINOR_CLUSTER_CONFIG_FAILED                  0x71 // 集群内设备配置失败
#define MINOR_CLUSTER_DISASTER_TOLERANCE_EXCEPT      0x72 // 集群容灾异常:集群CM选举失败,集群存储周期不足,集群带宽不足,集群通道资源不足,集群设备不足等
#define MINOR_CLUSTER_STORFULL_EXCEPTION             0x73 //集群硬盘满
#define MINOR_CLUSTER_VERSION_EXCEPTION              0x74  //集群版本异常
#define MINOR_CLUSTER_OFFLINENODE_EXCEPTION          0x75   //集群掉线数超限
#define MINOR_CLUSTER_RECORDCYCLE_EXCEPTION          0x76   //集群录像周期不足
#define MINOR_CLUSTER_IPCTRANSFER_EXCEPTION          0x77   //集群IPC迁移失败
#define MINOR_CLUSTER_IPCONFLICT_EXCEPTION           0x78   // 集群IP冲突，记录CM的IP地址

#define MINOR_GET_SUB_STREAM_FAILURE                 0x79//子码流取流失败
#define MINOR_HDD_SHM_DETECT_EXCEPTION               0x7a//硬盘SHM检测异常
#define MINOR_DEVICE_FORTIFY_FAILURE                 0x7b//前端设备报警布防失败
#define MINOR_EVENT_UPLOAD_EXCEPTION                 0x7c //事件发送异常（设备上传事件失败或者丢弃了）

//0x400-0x1000 门禁异常类型
#define MINOR_DEV_POWER_ON                     0x400  //设备上电启动
#define MINOR_DEV_POWER_OFF                    0x401  //设备掉电关闭
#define MINOR_WATCH_DOG_RESET                  0x402  //看门狗复位
#define MINOR_LOW_BATTERY                      0x403  //蓄电池电压低
#define MINOR_BATTERY_RESUME                   0x404  //蓄电池电压恢复正常
#define MINOR_AC_OFF                           0x405  //交流电断电
#define MINOR_AC_RESUME                        0x406  //交流电恢复
#define MINOR_NET_RESUME                       0x407  //网络恢复
#define MINOR_FLASH_ABNORMAL                   0x408  //FLASH读写异常
#define MINOR_CARD_READER_OFFLINE              0x409  //读卡器掉线
#define MINOR_CARD_READER_RESUME               0x40a  //读卡器掉线恢复                   
#define MINOR_INDICATOR_LIGHT_OFF              0x40b  //指示灯关闭
#define MINOR_INDICATOR_LIGHT_RESUME           0x40c  //指示灯恢复
#define MINOR_CHANNEL_CONTROLLER_OFF           0x40d  //通道控制器掉线
#define MINOR_CHANNEL_CONTROLLER_RESUME        0x40e  //通道控制器恢复
#define MINOR_SECURITY_MODULE_OFF               0x40f  //门控安全模块掉线
#define MINOR_SECURITY_MODULE_RESUME           0x410  //门控安全模块在线
#define MINOR_BATTERY_ELECTRIC_LOW             0x411  //电池电压低(仅人脸设备使用)
#define MINOR_BATTERY_ELECTRIC_RESUME          0x412  //电池电压恢复正常(仅人脸设备使用)
#define MINOR_LOCAL_CONTROL_NET_BROKEN           0x413  //就地控制器网络断开
#define MINOR_LOCAL_CONTROL_NET_RSUME           0x414  //就地控制器网络恢复
#define MINOR_MASTER_RS485_LOOPNODE_BROKEN     0x415  //主控RS485环路节点断开
#define MINOR_MASTER_RS485_LOOPNODE_RESUME     0x416  //主控RS485环路节点恢复
#define MINOR_LOCAL_CONTROL_OFFLINE            0x417  //就地控制器掉线
#define MINOR_LOCAL_CONTROL_RESUME             0x418  //就地控制器掉线恢复
#define MINOR_LOCAL_DOWNSIDE_RS485_LOOPNODE_BROKEN  0x419  //就地下行RS485环路断开
#define MINOR_LOCAL_DOWNSIDE_RS485_LOOPNODE_RESUME  0x41a  //就地下行RS485环路恢复
#define MINOR_DISTRACT_CONTROLLER_ONLINE       0x41b  //分控器在线
#define MINOR_DISTRACT_CONTROLLER_OFFLINE      0x41c  //分控器离线
#define MINOR_ID_CARD_READER_NOT_CONNECT       0x41d  //身份证阅读器未连接（智能专用）
#define MINOR_ID_CARD_READER_RESUME            0x41e  //身份证阅读器连接恢复（智能专用）
#define MINOR_FINGER_PRINT_MODULE_NOT_CONNECT  0x41f  //指纹模组未连接（智能专用）
#define MINOR_FINGER_PRINT_MODULE_RESUME       0x420  //指纹模组连接恢复（智能专用）
#define MINOR_CAMERA_NOT_CONNECT               0x421  //摄像头未连接
#define MINOR_CAMERA_RESUME                    0x422  //摄像头连接恢复
#define MINOR_COM_NOT_CONNECT                  0x423  //COM口未连接
#define MINOR_COM_RESUME                       0x424  //COM口连接恢复
#define MINOR_DEVICE_NOT_AUTHORIZE             0x425  //设备未授权
#define MINOR_PEOPLE_AND_ID_CARD_DEVICE_ONLINE 0x426  //人证设备在线
#define MINOR_PEOPLE_AND_ID_CARD_DEVICE_OFFLINE 0x427 //人证设备离线
#define MINOR_LOCAL_LOGIN_LOCK                 0x428  //本地登录锁定
#define MINOR_LOCAL_LOGIN_UNLOCK               0x429  //本地登录解锁
#define MINOR_SUBMARINEBACK_COMM_BREAK         0x42a  //与反潜回服务器通信断开
#define MINOR_SUBMARINEBACK_COMM_RESUME        0x42b  //与反潜回服务器通信恢复
#define MINOR_MOTOR_SENSOR_EXCEPTION           0x42c  //电机或传感器异常
#define MINOR_CAN_BUS_EXCEPTION                0x42d  //CAN总线异常
#define MINOR_CAN_BUS_RESUME                   0x42e  //CAN总线恢复
#define MINOR_GATE_TEMPERATURE_OVERRUN         0x42f  //闸机腔体温度超限
#define MINOR_IR_EMITTER_EXCEPTION             0x430  //红外对射异常
#define MINOR_IR_EMITTER_RESUME                0x431  //红外对射恢复
#define MINOR_LAMP_BOARD_COMM_EXCEPTION        0x432  //灯板通信异常
#define MINOR_LAMP_BOARD_COMM_RESUME           0x433  //灯板通信恢复
#define MINOR_IR_ADAPTOR_COMM_EXCEPTION        0x434  //红外转接板通信异常
#define MINOR_IR_ADAPTOR_COMM_RESUME           0x435  //红外转接板通信恢复
#define MINOR_PRINTER_ONLINE                    0x436  //打印机在线
#define MINOR_PRINTER_OFFLINE                    0x437  //打印机离线
#define MINOR_4G_MOUDLE_ONLINE       0x438  //4G模块在线
#define MINOR_4G_MOUDLE_OFFLINE         0x439  //4G模块离线

#define MINOR_EXCEPTION_CUSTOM1                 0x900  //门禁自定义异常1
#define MINOR_EXCEPTION_CUSTOM2                 0x901  //门禁自定义异常2
#define MINOR_EXCEPTION_CUSTOM3                 0x902  //门禁自定义异常3
#define MINOR_EXCEPTION_CUSTOM4                 0x903  //门禁自定义异常4
#define MINOR_EXCEPTION_CUSTOM5                 0x904  //门禁自定义异常5
#define MINOR_EXCEPTION_CUSTOM6                 0x905  //门禁自定义异常6
#define MINOR_EXCEPTION_CUSTOM7                 0x906  //门禁自定义异常7
#define MINOR_EXCEPTION_CUSTOM8                 0x907  //门禁自定义异常8
#define MINOR_EXCEPTION_CUSTOM9                 0x908  //门禁自定义异常9
#define MINOR_EXCEPTION_CUSTOM10                0x909  //门禁自定义异常10
#define MINOR_EXCEPTION_CUSTOM11                0x90a  //门禁自定义异常11
#define MINOR_EXCEPTION_CUSTOM12                0x90b  //门禁自定义异常12
#define MINOR_EXCEPTION_CUSTOM13                0x90c  //门禁自定义异常13
#define MINOR_EXCEPTION_CUSTOM14                0x90d  //门禁自定义异常14
#define MINOR_EXCEPTION_CUSTOM15                0x90e  //门禁自定义异常15
#define MINOR_EXCEPTION_CUSTOM16                0x90f  //门禁自定义异常16
#define MINOR_EXCEPTION_CUSTOM17                0x910  //门禁自定义异常17
#define MINOR_EXCEPTION_CUSTOM18                0x911  //门禁自定义异常18
#define MINOR_EXCEPTION_CUSTOM19                0x912  //门禁自定义异常19
#define MINOR_EXCEPTION_CUSTOM20                0x913  //门禁自定义异常20
#define MINOR_EXCEPTION_CUSTOM21                0x914  //门禁自定义异常21
#define MINOR_EXCEPTION_CUSTOM22                0x915  //门禁自定义异常22
#define MINOR_EXCEPTION_CUSTOM23                0x916  //门禁自定义异常23
#define MINOR_EXCEPTION_CUSTOM24                0x917  //门禁自定义异常24
#define MINOR_EXCEPTION_CUSTOM25                0x918  //门禁自定义异常25
#define MINOR_EXCEPTION_CUSTOM26                0x919  //门禁自定义异常26
#define MINOR_EXCEPTION_CUSTOM27                0x91a  //门禁自定义异常27
#define MINOR_EXCEPTION_CUSTOM28                0x91b  //门禁自定义异常28
#define MINOR_EXCEPTION_CUSTOM29                0x91c  //门禁自定义异常29
#define MINOR_EXCEPTION_CUSTOM30                0x91d  //门禁自定义异常30
#define MINOR_EXCEPTION_CUSTOM31                0x91e  //门禁自定义异常31
#define MINOR_EXCEPTION_CUSTOM32                0x91f  //门禁自定义异常32
#define MINOR_EXCEPTION_CUSTOM33                0x920  //门禁自定义异常33
#define MINOR_EXCEPTION_CUSTOM34                0x921  //门禁自定义异常34
#define MINOR_EXCEPTION_CUSTOM35                0x922  //门禁自定义异常35
#define MINOR_EXCEPTION_CUSTOM36                0x923  //门禁自定义异常36
#define MINOR_EXCEPTION_CUSTOM37                0x924  //门禁自定义异常37
#define MINOR_EXCEPTION_CUSTOM38                0x925  //门禁自定义异常38
#define MINOR_EXCEPTION_CUSTOM39                0x926  //门禁自定义异常39
#define MINOR_EXCEPTION_CUSTOM40                0x927  //门禁自定义异常40
#define MINOR_EXCEPTION_CUSTOM41                0x928  //门禁自定义异常41
#define MINOR_EXCEPTION_CUSTOM42                0x929  //门禁自定义异常42
#define MINOR_EXCEPTION_CUSTOM43                0x92a  //门禁自定义异常43
#define MINOR_EXCEPTION_CUSTOM44                0x92b  //门禁自定义异常44
#define MINOR_EXCEPTION_CUSTOM45                0x92c  //门禁自定义异常45
#define MINOR_EXCEPTION_CUSTOM46                0x92d  //门禁自定义异常46
#define MINOR_EXCEPTION_CUSTOM47                0x92e  //门禁自定义异常47
#define MINOR_EXCEPTION_CUSTOM48                0x92f  //门禁自定义异常48
#define MINOR_EXCEPTION_CUSTOM49                0x930  //门禁自定义异常49
#define MINOR_EXCEPTION_CUSTOM50                0x931  //门禁自定义异常50
#define MINOR_EXCEPTION_CUSTOM51                0x932  //门禁自定义异常51
#define MINOR_EXCEPTION_CUSTOM52                0x933  //门禁自定义异常52
#define MINOR_EXCEPTION_CUSTOM53                0x934  //门禁自定义异常53
#define MINOR_EXCEPTION_CUSTOM54                0x935  //门禁自定义异常54
#define MINOR_EXCEPTION_CUSTOM55                0x936  //门禁自定义异常55
#define MINOR_EXCEPTION_CUSTOM56                0x937  //门禁自定义异常56
#define MINOR_EXCEPTION_CUSTOM57                0x938  //门禁自定义异常57
#define MINOR_EXCEPTION_CUSTOM58                0x939  //门禁自定义异常58
#define MINOR_EXCEPTION_CUSTOM59                0x93a  //门禁自定义异常59
#define MINOR_EXCEPTION_CUSTOM60                0x93b  //门禁自定义异常60
#define MINOR_EXCEPTION_CUSTOM61                0x93c  //门禁自定义异常61
#define MINOR_EXCEPTION_CUSTOM62                0x93d  //门禁自定义异常62
#define MINOR_EXCEPTION_CUSTOM63                0x93e  //门禁自定义异常63
#define MINOR_EXCEPTION_CUSTOM64                0x93f  //门禁自定义异常64
#define MINOR_SWITCH_WIRED_NETWORK            0x950    //切换有线网络
#define MINOR_SWITCH_WIRELESS_NETWORK         0x951    //切换无线网络
#define MINOR_LOCK_ONLINE_RESUME              0x952    //智能锁恢复上线

//[add]by silujie 2013-3-22 14:16
//0x2000~0x3fff 为设备报警日志
//0x4000~0x5000 为设备异常日志
#define MINOR_SUBSYSTEM_IP_CONFLICT        0x4000        //子板IP冲突
#define MINOR_SUBSYSTEM_NET_BROKEN        0x4001        //子板断网
#define    MINOR_FAN_ABNORMAL                0x4002        //风扇异常
#define    MINOR_BACKPANEL_TEMPERATURE_ABNORMAL        0x4003        //背板温度异常

#define    MINOR_SDCARD_ABNORMAL            0x4004        //SD卡不健康
#define    MINOR_SDCARD_DAMAGE                0x4005        //SD卡损坏
#define    MINOR_POC_ABNORMAL				0x4006   //设备POC模块异常
#define MINOR_MAIN_POWER_FAULT              0x4007 //主电故障
#define MINOR_BACK_UP_POWER_FAULT           0x4008 //备电故障
#define MINOR_TAMPER_FAULT                  0x4009 //防拆故障
#define MINOR_RS232_FAULT                   0x400a //232总线故障
#define MINOR_RS485_FAULT                   0x400b //485总线故障
#define MINOR_LAN_STATUS_FAULT              0x400c //LAN网线接入状态故障
#define MINOR_LAN_LINK1_FAULT               0x400d //LAN链路1故障
#define MINOR_LAN_LINK2_FAULT               0x400e //LAN链路2故障
#define MINOR_SIM_CARD_STATUS_FAULT         0x400f //4G-SIM卡状态故障
#define MINOR_4G_LINK1_FAULT                0x4010 //4G链路1故障
#define MINOR_4G_LINK2_FAULT                0x4011 //4G链路2故障
#define MINOR_OTHER_FAULT                   0x4012 //其他故障

//2018-04-23 通用物联网关异常日志类型
#define MINOR_ALARMHOST_WDT_RESET 			    0x1003    //WDT 复位
#define MINOR_ALARMHOST_RTC_EXCEPTION 			0x1007    //RTC实时时钟异常

#define MINOR_ALARMHOST_TEL_LINE_CONNECT_FAILURE 	0x100a    //电话线连接断
#define MINOR_ALARMHOST_TEL_LINE_CONNECT_RESTORE 	0x100b    //电话线连接恢复
#define MINOR_ALARMHOST_EXPANDER_BUS_LOSS 		0x100c    //扩展总线模块掉线
#define MINOR_ALARMHOST_EXPANDER_BUS_RESTORE 	0x100d    //扩展总线模块掉线恢复
#define MINOR_ALARMHOST_KEYPAD_BUS_LOSS 			0x100e    //键盘总线模块掉线
#define MINOR_ALARMHOST_KEYPAD_BUS_RESTORE 		0x100f    //键盘总线模块掉线恢复
#define MINOR_ALARMHOST_SENSOR_FAILURE 			0x1010    //模拟量传感器故障
#define MINOR_ALARMHOST_SENSOR_RESTORE 			0x1011    //模拟量传感器恢复
#define MINOR_ALARMHOST_RS485_CONNECT_FAILURE 	0x1012    //RS485通道连接断
#define MINOR_ALARMHOST_RS485_CONNECT_RESTORE 	0x1013    //RS485通道连接断恢复

//“有线网络异常”和“有线网络异常恢复”这两个日志跟“网络连接断”“网络连接恢复”这两个日志时一样的，且没有设备支持“有线网络异常”和“有线网络异常恢复”这两种类型。
#define	MINOR_ALARMHOST_WIRED_NETWORK_ABNORMAL 	0x1015    //有线网络异常
#define	MINOR_ALARMHOST_WIRED_NETWORK_RESTORE 		0x1016    //有线网络恢复正常
#define	MINOR_ALARMHOST_GPRS_ABNORMAL 				0x1017    //GPRS通信异常
#define	MINOR_ALARMHOST_GPRS_RESTORE 					0x1018    //GPRS恢复正常
#define	MINOR_ALARMHOST_3G_ABNORMAL 					0x1019    //3G通信异常
#define	MINOR_ALARMHOST_3G_RESTORE 					0x101a    //3G恢复正常
#define	MINOR_ALARMHOST_SIM_CARD_ABNORMAL 			0x101b    //SIM卡异常
#define	MINOR_ALARMHOST_SIM_CARD_RESTORE 			0x101c    //SIM卡恢复正常

#define	MINOR_FORMAT_HDD_ERROR  0x1026 //远程格式化硬盘失败
#define	MINOR_USB_ERROR		0x1027    //USB通信故障
#define	MINOR_USB_RESTORE		0x1028    //USB通信故障恢复
#define	MINOR_PRINT_ERROR	  0x1029    //打印机故障
#define	MINOR_PRINT_RESTORE   0x1030    //打印机故障恢复
#define  MINOR_ALARMHOST_SUBSYSTEM_COMMUNICATION_ERROR    0x1031  //子板通讯错误

#define  MINOR_MCU_RESTART    0x1035    //MCU重启
#define  MINOR_GPRS_MODULE_FAULT    0x1036  //GPRS模块故障
#define  MINOR_TELEPHONE_MODULE_FAULT    0x1037     //电话模块故障
#define  MINOR_WIFI_ABNORMAL  0x1038    //WIFI通信异常
#define  MINOR_WIFI_RESTORE    0x1039    //WIFI恢复正常
#define  MINOR_RF_ABNORMAL   0x103a    //RF信号异常
#define  MINOR_RF_RESTORE     0x103b   //RF信号恢复正常
#define  MINOR_DETECTOR_ONLINE   0x103c    //探测器在线
#define  MINOR_DETECTOR_OFFLINE    0x103d    //探测器离线
#define  MINOR_DETECTOR_BATTERY_NORMAL    0x103e  //探测器电量正常        
#define  MINOR_DETECTOR_BATTERY_LOW 0x103f  //探测器电量欠压
#define  MINOR_DATA_TRAFFIC_OVERFLOW    0x1040    //流量超额
#define  MINOR_ALARMHOST_ZONE_MODULE_LOSS     0x1041    //防区模块掉线
#define  MINOR_ALARMHOST_ZONE_MODULE_RESTORE     0x1042    //防区模块掉线恢复
#define  MINOR_WIRELESS_OUTPUT_LOSS     0x1043    //无线输出模块离线
#define  MINOR_WIRELESS_OUTPUT_RESTORE    0x1044    //无线输出模块恢复在线
#define  MINOR_WIRELESS_REPEATER_LOSS    0x1045    //无线中继器离线
#define  MINOR_WIRELESS_REPEATER_RESTORE    0x1046    //无线中继器恢复在线
#define  MINOR_ALARMHOST_TRIGGER_MODULE_LOSS     0x1047    //触发器模块掉线
#define  MINOR_ALARMHOST_TRIGGER_MODULE_RESTORE    0x1048    //触发器模块掉线恢复
#define  MINOR_ALARMHOST_WIRELESS_SIREN_LOSS    0x1049    //无线警号离线
#define  MINOR_ALARMHOST_WIRELESS_SIREN_RESTORE     0x104a    //无线警号恢复在线
#define  MINOR_TX1_SUB_SYSTEM_EXCEPTION             0x1050   /*TX1子系统异常*/
#define MINOR_TX1_REBOOT_EXCEPTION       0x1051   /*TX1系统异常重启*/
#define MINOR_TX1_SUB_SYSTEM_LOSS      0x1052   /*智能子系统异常离线*/
#define MINOR_TX1_SUB_SYSTEM_RESTORE     0x1053   /*智能子系统离线恢复*/
#define MINOR_WIRELESS_SPEED_EXCEPTION   0x1054   //无线传输速率异常



/* 操作 */
//主类型
#define MAJOR_OPERATION                    0x3

//次类型
#define MINOR_VCA_MOTIONEXCEPTION        0x29  //智能侦测异常
#define MINOR_START_DVR                    0x41    /* 开机 */
#define MINOR_STOP_DVR                    0x42    /* 关机 */
#define MINOR_STOP_ABNORMAL                0x43    /* 异常关机 */
#define MINOR_REBOOT_DVR                0x44    /*本地重启设备*/

#define MINOR_LOCAL_LOGIN                0x50    /* 本地登陆 */
#define MINOR_LOCAL_LOGOUT                0x51    /* 本地注销登陆 */
#define MINOR_LOCAL_CFG_PARM            0x52    /* 本地配置参数 */
#define MINOR_LOCAL_PLAYBYFILE          0x53    /* 本地按文件回放或下载 */
#define MINOR_LOCAL_PLAYBYTIME          0x54    /* 本地按时间回放或下载*/
#define MINOR_LOCAL_START_REC            0x55    /* 本地开始录像 */
#define MINOR_LOCAL_STOP_REC            0x56    /* 本地停止录像 */
#define MINOR_LOCAL_PTZCTRL                0x57    /* 本地云台控制 */
#define MINOR_LOCAL_PREVIEW                0x58    /* 本地预览 (保留不使用)*/
#define MINOR_LOCAL_MODIFY_TIME         0x59    /* 本地修改时间(保留不使用) */
#define MINOR_LOCAL_UPGRADE             0x5a    /* 本地升级 */
#define MINOR_LOCAL_RECFILE_OUTPUT      0x5b    /* 本地备份录象文件 */
#define MINOR_LOCAL_FORMAT_HDD          0x5c    /* 本地初始化硬盘 */
#define MINOR_LOCAL_CFGFILE_OUTPUT      0x5d    /* 导出本地配置文件 */
#define MINOR_LOCAL_CFGFILE_INPUT       0x5e    /* 导入本地配置文件 */
#define MINOR_LOCAL_COPYFILE            0x5f    /* 本地备份文件 */
#define MINOR_LOCAL_LOCKFILE            0x60    /* 本地锁定录像文件 */
#define MINOR_LOCAL_UNLOCKFILE          0x61    /* 本地解锁录像文件 */
#define MINOR_LOCAL_DVR_ALARM           0x62    /* 本地手动清除和触发报警*/
#define MINOR_IPC_ADD                   0x63    /* 本地添加IPC */
#define MINOR_IPC_DEL                   0x64    /* 本地删除IPC */
#define MINOR_IPC_SET                   0x65    /* 本地设置IPC */
#define MINOR_LOCAL_START_BACKUP        0x66    /* 本地开始备份 */
#define MINOR_LOCAL_STOP_BACKUP            0x67    /* 本地停止备份*/
#define MINOR_LOCAL_COPYFILE_START_TIME 0x68    /* 本地备份开始时间*/
#define MINOR_LOCAL_COPYFILE_END_TIME    0x69    /* 本地备份结束时间*/
#define MINOR_LOCAL_ADD_NAS             0x6a    /*本地添加网络硬盘 （nfs、iscsi）*/
#define MINOR_LOCAL_DEL_NAS             0x6b    /* 本地删除nas盘 （nfs、iscsi）*/
#define MINOR_LOCAL_SET_NAS             0x6c    /* 本地设置nas盘 （nfs、iscsi）*/
#define MINOR_LOCAL_RESET_PASSWD        0x6d    /* 本地恢复管理员默认密码*/ 

#define MINOR_REMOTE_LOGIN                0x70    /* 远程登录 */
#define MINOR_REMOTE_LOGOUT                0x71    /* 远程注销登陆 */
#define MINOR_REMOTE_START_REC            0x72    /* 远程开始录像 */
#define MINOR_REMOTE_STOP_REC            0x73    /* 远程停止录像 */
#define MINOR_START_TRANS_CHAN            0x74    /* 开始透明传输 */
#define MINOR_STOP_TRANS_CHAN            0x75    /* 停止透明传输 */
#define MINOR_REMOTE_GET_PARM            0x76    /* 远程获取参数 */
#define MINOR_REMOTE_CFG_PARM            0x77    /* 远程配置参数 */
#define MINOR_REMOTE_GET_STATUS         0x78    /* 远程获取状态 */
#define MINOR_REMOTE_ARM                0x79    /* 远程布防 */
#define MINOR_REMOTE_DISARM                0x7a    /* 远程撤防 */
#define MINOR_REMOTE_REBOOT                0x7b    /* 远程重启 */
#define MINOR_START_VT                    0x7c    /* 开始语音对讲 */
#define MINOR_STOP_VT                    0x7d    /* 停止语音对讲 */
#define MINOR_REMOTE_UPGRADE            0x7e    /* 远程升级 */
#define MINOR_REMOTE_PLAYBYFILE         0x7f    /* 远程按文件回放 */
#define MINOR_REMOTE_PLAYBYTIME         0x80    /* 远程按时间回放 */
#define MINOR_REMOTE_PTZCTRL            0x81    /* 远程云台控制 */
#define MINOR_REMOTE_FORMAT_HDD         0x82    /* 远程格式化硬盘 */
#define MINOR_REMOTE_STOP               0x83    /* 远程关机 */
#define MINOR_REMOTE_LOCKFILE            0x84    /* 远程锁定文件 */
#define MINOR_REMOTE_UNLOCKFILE         0x85    /* 远程解锁文件 */
#define MINOR_REMOTE_CFGFILE_OUTPUT     0x86    /* 远程导出配置文件 */
#define MINOR_REMOTE_CFGFILE_INTPUT     0x87    /* 远程导入配置文件 */
#define MINOR_REMOTE_RECFILE_OUTPUT     0x88    /* 远程导出录象文件 */
#define MINOR_REMOTE_DVR_ALARM          0x89    /* 远程手动清除和触发报警*/
#define MINOR_REMOTE_IPC_ADD            0x8a    /* 远程添加IPC */
#define MINOR_REMOTE_IPC_DEL            0x8b    /* 远程删除IPC */
#define MINOR_REMOTE_IPC_SET            0x8c    /* 远程设置IPC */
#define MINOR_REBOOT_VCA_LIB            0x8d    /*重启智能库*/
#define MINOR_REMOTE_ADD_NAS            0x8e   /* 远程添加nas盘 （nfs、iscsi）*/
#define MINOR_REMOTE_DEL_NAS            0x8f   /* 远程删除nas盘 （nfs、iscsi）*/
#define MINOR_REMOTE_SET_NAS            0x90   /* 远程设置nas盘 （nfs、iscsi）*/
#define MINOR_LOCAL_OPERATE_LOCK        0x9d            /* 本地操作锁定             */
#define MINOR_LOCAL_OPERATE_UNLOCK      0x9e            /* 本地操作解除锁定         */
#define MINOR_REMOTE_DELETE_HDISK       0x9a            /* 远程删除异常不存在的硬盘 */
#define MINOR_REMOTE_LOAD_HDISK         0x9b            /* 远程加载硬盘             */
#define MINOR_REMOTE_UNLOAD_HDISK       0x9c            /* 远程卸载硬盘   */


//2010-05-26 增加审讯DVR日志类型
#define MINOR_LOCAL_START_REC_CDRW      0x91   /* 本地开始讯问 */
#define MINOR_LOCAL_STOP_REC_CDRW       0x92   /* 本地停止讯问 */
#define MINOR_REMOTE_START_REC_CDRW     0x93   /* 远程开始讯问 */
#define MINOR_REMOTE_STOP_REC_CDRW      0x94   /* 远程停止讯问 */

#define MINOR_LOCAL_PIC_OUTPUT            0x95   /* 本地备份图片文件 */ 
#define MINOR_REMOTE_PIC_OUTPUT            0x96   /* 远程备份图片文件 */

//2011-07-26 增加81审讯DVR日志类型
#define MINOR_LOCAL_INQUEST_RESUME      0x97   /* 本地恢复审讯事件*/
#define MINOR_REMOTE_INQUEST_RESUME     0x98   /* 远程恢复审讯事件*/

//2013-01-23 增加86高清审讯NVR操作日志
#define MINOR_LOCAL_ADD_FILE            0x99        /*本地导入文件*/
#define MINOR_LOCAL_DEL_FILE            0x9f        /*本地删除审讯*/
#define MINOR_REMOTE_INQUEST_ADD_FILE   0x100       /*远程导入文件*/ 

//2009-12-16 增加视频综合平台日志类型
#define MINOR_SUBSYSTEMREBOOT           0xa0    /*视频综合平台：dm6467 正常重启*/
#define MINOR_MATRIX_STARTTRANSFERVIDEO 0xa1    /*视频综合平台：矩阵切换开始传输图像*/
#define MINOR_MATRIX_STOPTRANSFERVIDEO    0xa2    /*视频综合平台：矩阵切换停止传输图像*/
#define MINOR_REMOTE_SET_ALLSUBSYSTEM   0xa3    /*视频综合平台：设置所有6467子系统信息*/
#define MINOR_REMOTE_GET_ALLSUBSYSTEM   0xa4    /*视频综合平台：获取所有6467子系统信息*/
#define MINOR_REMOTE_SET_PLANARRAY      0xa5    /*视频综合平台：设置计划轮巡组*/
#define MINOR_REMOTE_GET_PLANARRAY      0xa6    /*视频综合平台：获取计划轮巡组*/
#define MINOR_MATRIX_STARTTRANSFERAUDIO 0xa7    /*视频综合平台：矩阵切换开始传输音频*/
#define MINOR_MATRIX_STOPRANSFERAUDIO   0xa8    /*视频综合平台：矩阵切换停止传输音频*/
#define MINOR_LOGON_CODESPITTER         0xa9    /*视频综合平台：登陆码分器*/
#define MINOR_LOGOFF_CODESPITTER        0xaa    /*视频综合平台：退出码分器*/

//2010-01-22 增加视频综合平台中解码器操作日志
#define MINOR_START_DYNAMIC_DECODE         0xb0    /*开始动态解码*/
#define MINOR_STOP_DYNAMIC_DECODE        0xb1    /*停止动态解码*/
#define MINOR_GET_CYC_CFG                0xb2    /*获取解码器通道轮巡配置*/
#define MINOR_SET_CYC_CFG                0xb3    /*设置解码通道轮巡配置*/
#define MINOR_START_CYC_DECODE            0xb4    /*开始轮巡解码*/
#define MINOR_STOP_CYC_DECODE            0xb5    /*停止轮巡解码*/
#define MINOR_GET_DECCHAN_STATUS        0xb6    /*获取解码通道状态*/
#define MINOR_GET_DECCHAN_INFO            0xb7    /*获取解码通道当前信息*/
#define MINOR_START_PASSIVE_DEC            0xb8    /*开始被动解码*/
#define MINOR_STOP_PASSIVE_DEC            0xb9    /*停止被动解码*/
#define MINOR_CTRL_PASSIVE_DEC            0xba    /*控制被动解码*/
#define MINOR_RECON_PASSIVE_DEC            0xbb    /*被动解码重连*/
#define MINOR_GET_DEC_CHAN_SW            0xbc    /*获取解码通道总开关*/
#define MINOR_SET_DEC_CHAN_SW            0xbd    /*设置解码通道总开关*/
#define MINOR_CTRL_DEC_CHAN_SCALE        0xbe    /*解码通道缩放控制*/
#define MINOR_SET_REMOTE_REPLAY            0xbf    /*设置远程回放*/
#define MINOR_GET_REMOTE_REPLAY            0xc0    /*获取远程回放状态*/
#define MINOR_CTRL_REMOTE_REPLAY        0xc1    /*远程回放控制*/
#define MINOR_SET_DISP_CFG                0xc2    /*设置显示通道*/
#define MINOR_GET_DISP_CFG                0xc3    /*获取显示通道设置*/
#define MINOR_SET_PLANTABLE                0xc4    /*设置计划轮巡表*/
#define MINOR_GET_PLANTABLE                0xc5    /*获取计划轮巡表*/
#define MINOR_START_PPPPOE                0xc6    /*开始PPPoE连接*/
#define MINOR_STOP_PPPPOE                0xc7    /*结束PPPoE连接*/
#define MINOR_UPLOAD_LOGO                0xc8    /*上传LOGO*/
//推模式操作日志
#define MINOR_LOCAL_PIN                    0xc9    /* 本地PIN功能操作 */
#define MINOR_LOCAL_DIAL                0xca    /* 本地手动启动断开拨号 */    
#define MINOR_SMS_CONTROL                0xcb    /* 短信控制上下线 */    
#define MINOR_CALL_ONLINE                0xcc    /* 呼叫控制上线 */    
#define MINOR_REMOTE_PIN                0xcd    /* 远程PIN功能操作 */

//2010-12-16 报警板日志
#define MINOR_REMOTE_BYPASS             0xd0    /* 远程旁路*/
#define MINOR_REMOTE_UNBYPASS           0xd1    /* 远程旁路恢复*/
#define MINOR_REMOTE_SET_ALARMIN_CFG    0xd2    /* 远程设置报警输入参数*/
#define MINOR_REMOTE_GET_ALARMIN_CFG    0xd3    /* 远程获取报警输入参数*/
#define MINOR_REMOTE_SET_ALARMOUT_CFG   0xd4    /* 远程设置报警输出参数*/
#define MINOR_REMOTE_GET_ALARMOUT_CFG   0xd5    /* 远程获取报警输出参数*/
#define MINOR_REMOTE_ALARMOUT_OPEN_MAN  0xd6    /* 远程手动开启报警输出*/
#define MINOR_REMOTE_ALARMOUT_CLOSE_MAN 0xd7    /* 远程手动关闭报警输出*/
#define MINOR_REMOTE_ALARM_ENABLE_CFG   0xd8    /* 远程设置报警主机的RS485串口使能状态*/
#define MINOR_DBDATA_OUTPUT                0xd9    /* 导出数据库记录 */
#define MINOR_DBDATA_INPUT                0xda    /* 导入数据库记录 */
#define MINOR_MU_SWITCH                    0xdb    /* 级联切换 */
#define MINOR_MU_PTZ                    0xdc    /* 级联PTZ控制 */
#define MINOR_DELETE_LOGO               0xdd    /* 删除logo */
#define MINOR_REMOTE_INQUEST_DEL_FILE   0xde       /*远程删除文件*/

#define MINOR_LOCAL_CONF_REB_RAID       0x101           /*本地配置自动重建*/
#define MINOR_LOCAL_CONF_SPARE          0x102           /*本地配置热备*/
#define MINOR_LOCAL_ADD_RAID            0x103           /*本地创建阵列*/
#define MINOR_LOCAL_DEL_RAID            0x104           /*本地删除阵列*/
#define MINOR_LOCAL_MIG_RAID            0x105           /*本地迁移阵列*/
#define MINOR_LOCAL_REB_RAID            0x106           /* 本地手动重建阵列*/
#define MINOR_LOCAL_QUICK_CONF_RAID     0x107           /*本地一键配置*/
#define MINOR_LOCAL_ADD_VD              0x108           /*本地创建虚拟磁盘*/
#define MINOR_LOCAL_DEL_VD              0x109           /*本地删除虚拟磁盘*/
#define MINOR_LOCAL_RP_VD               0x10a           /*本地修复虚拟磁盘*/
#define MINOR_LOCAL_FORMAT_EXPANDVD     0X10b           /*本地扩展虚拟磁盘扩容*/
#define MINOR_LOCAL_RAID_UPGRADE        0X10c           /*本地raid卡升级*/  
#define MINOR_LOCAL_STOP_RAID           0x10d           /*本地暂停RAID操作(即安全拔盘)*/  
#define MINOR_REMOTE_CONF_REB_RAID      0x111           /*远程配置自动重建*/
#define MINOR_REMOTE_CONF_SPARE         0x112            /*远程配置热备*/
#define MINOR_REMOTE_ADD_RAID           0x113           /*远程创建阵列*/
#define MINOR_REMOTE_DEL_RAID           0x114           /*远程删除阵列*/
#define MINOR_REMOTE_MIG_RAID           0x115           /*远程迁移阵列*/
#define MINOR_REMOTE_REB_RAID           0x116           /* 远程手动重建阵列*/
#define MINOR_REMOTE_QUICK_CONF_RAID    0x117           /*远程一键配置*/
#define MINOR_REMOTE_ADD_VD             0x118           /*远程创建虚拟磁盘*/
#define MINOR_REMOTE_DEL_VD             0x119           /*远程删除虚拟磁盘*/
#define MINOR_REMOTE_RP_VD              0x11a           /*远程修复虚拟磁盘*/
#define MINOR_REMOTE_FORMAT_EXPANDVD    0X11b           /*远程虚拟磁盘扩容*/
#define MINOR_REMOTE_RAID_UPGRADE       0X11c           /*远程raid卡升级*/  
#define MINOR_REMOTE_STOP_RAID          0x11d           /*远程暂停RAID操作(即安全拔盘)*/
#define MINOR_LOCAL_START_PIC_REC       0x121          /*本地开始抓图*/ 
#define MINOR_LOCAL_STOP_PIC_REC        0x122          /*本地停止抓图*/
#define MINOR_LOCAL_SET_SNMP            0x125           /*本地配置SNMP*/
#define MINOR_LOCAL_TAG_OPT             0x126          /*本地标签操作*/
#define MINOR_REMOTE_START_PIC_REC      0x131          /*远程开始抓图*/
#define MINOR_REMOTE_STOP_PIC_REC       0x132           /*远程停止抓图*/
#define MINOR_REMOTE_SET_SNMP           0x135            /*远程配置SNMP*/
#define MINOR_REMOTE_TAG_OPT            0x136             /*远程标签操作*/  
#define MINOR_REMOTE_LOGIN_LOCK         0x137           //远程登录锁定
#define MINOR_REMOTE_LOGIN_UNLOCK       0x138          //远程登录解锁
// 9000 v2.2.0
#define MINOR_LOCAL_VOUT_SWITCH         0x140   /* 本地输出口切换操作*/
#define MINOR_STREAM_CABAC                 0x141   /* 码流压缩性能选项配置操作*/

//Netra 3.0.0
#define MINOR_LOCAL_SPARE_OPT           0x142   /*本地N+1 热备相关操作*/
#define MINOR_REMOTE_SPARE_OPT            0x143   /*远程N+1 热备相关操作*/
#define MINOR_LOCAL_IPCCFGFILE_OUTPUT    0x144      /* 本地导出ipc配置文件*/
#define MINOR_LOCAL_IPCCFGFILE_INPUT      0x145   /* 本地导入ipc配置文件 */
#define MINOR_LOCAL_IPC_UPGRADE         0x146   /* 本地升级IPC */
#define MINOR_REMOTE_IPCCFGFILE_OUTPUT  0x147   /* 远程导出ipc配置文件*/
#define MINOR_REMOTE_IPCCFGFILE_INPUT   0x148   /* 远程导入ipc配置文件*/
#define MINOR_REMOTE_IPC_UPGRADE        0x149   /* 远程升级IPC */

#define MINOR_LOCAL_UNLOAD_HDISK        0x150     /*本地卸载硬盘*/
#define MINOR_LOCAL_AUDIO_MIX           0x151     /*本地配置音频混音参数*/
#define MINOR_REMOTE_AUDIO_MIX          0x152     /*远程配置音频混音参数*/
#define MINOR_LOCAL_TRIAL_PAUSE         0x153     /*本地暂停讯问*/
#define MINOR_LOCAL_TRIAL_RESUME        0x154     /*本地继续讯问*/
#define MINOR_REMOTE_TRIAL_PAUSE        0x155     /*远程暂停讯问*/
#define MINOR_REMOTE_TRIAL_RESUME       0x156     /*远程继续讯问*/
#define MINOR_REMOTE_MODIFY_VERIFICATION_CODE   0x157 /*修改平台的验证码*/

#define MINOR_LOCAL_MAKECALL            0x180          /*本地呼叫*/
#define MINOR_LOCAL_REJECTCALL          0x181          /*本地拒接*/
#define MINOR_LOCAL_ANSWERCALL          0x182          /*本地接听*/
#define MINOR_LOCAL_HANGUPCALL          0x183          /*本地挂断*/

#define MINOR_REMOTE_MAKECALL           0x188          /*远程呼叫*/
#define MINOR_REMOTE_REJECTCALL         0x189          /*远程拒接*/
#define MINOR_REMOTE_ANSWERCALL         0x18a          /*远程接听*/
#define MINOR_REMOTE_HANGUPCALL         0x18b          /*远程挂断*/

#define MINOR_SET_MULTI_MASTER          0x201    /*设置大屏主屏*/
#define MINOR_SET_MULTI_SLAVE           0x202    /*设置大屏子屏*/
#define MINOR_CANCEL_MULTI_MASTER       0x203    /*取消大屏主屏*/
#define MINOR_CANCEL_MULTI_SLAVE        0x204    /*取消大屏子屏*/

#define MINOR_DISPLAY_LOGO                0x205    /*显示LOGO*/
#define MINOR_HIDE_LOGO                 0x206    /*隐藏LOGO*/
#define MINOR_SET_DEC_DELAY_LEVEL       0x207    /*解码通道延时级别设置*/
#define MINOR_SET_BIGSCREEN_DIPLAY_AREA 0x208    /*设置大屏显示区域*/
#define MINOR_CUT_VIDEO_SOURCE          0x209    /*大屏视频源切割设置*/
#define MINOR_SET_BASEMAP_AREA          0x210    /*大屏底图区域设置*/
#define MINOR_DOWNLOAD_BASEMAP          0x211    /*下载大屏底图*/
#define MINOR_CUT_BASEMAP               0x212    /*底图切割配置*/
#define MINOR_CONTROL_ELEC_ENLARGE      0x213    /*电子放大操作(放大或还原)*/
#define MINOR_SET_OUTPUT_RESOLUTION     0x214    /*显示输出分辨率设置*/
#define MINOR_SET_TRANCSPARENCY         0X215    /*图层透明度设置*/
#define MINOR_SET_OSD                   0x216    /*显示OSD设置*/
#define MINOR_RESTORE_DEC_STATUS        0x217    /*恢复初始状态(场景切换时，解码恢复初始状态)*/

//2011-11-11 增加大屏控制器操作日志次类型
#define MINOR_SCREEN_OPEN_SCREEN        0x218   //打开屏幕
#define MINOR_SCREEN_CLOSE_SCREEN       0x219   //关闭屏幕
#define MINOR_SCREEN_SWITCH_SIGNAL      0x21a   //信号源切换
#define MINOR_SCREEN_MODIFY_NETWORK     0x21b   //配置网络参数
#define MINOR_SCREEN_MODIFY_LEDRES      0x21c   //配置输出口LED分辨率
#define MINOR_SCREEN_SHOW_NORMAL        0x21d   //配置窗口普通显示模式
#define MINOR_SCREEN_SHOW_TILE          0x21e   //配置窗口平铺显示模式
#define MINOR_SCREEN_DEC_NORMAL         0x21f   //配置普通解码模式
#define MINOR_SCREEN_DEC_LOWLATENCY     0x220   //配置低延时解码模式
#define MINOR_SCREEN_MODIFY_SELFRES     0x221   //配置信号源自定义分辨率
#define MINOR_SCREEN_OUTPUT_POSITION    0x222   //输出口关联屏幕
#define MINOR_SCREEN_IMAGE_ENHANCE      0x223   //图像增强
#define MINOR_SCREEN_JOIN_SIGNAL        0x224   //信号源拼接
#define MINOR_SCREEN_SIGNAL_OSD         0x225   //信号源字符叠加
#define MINOR_SCREEN_ASSOCIATED_INTERACTION 0x226   //信号源关联多屏互动服务器
#define MINOR_SCREEN_MODIFY_MATRIX      0x227   //配置矩阵参数
#define MINOR_SCREEN_WND_TOP_KEEP       0x228   //窗口置顶保持
#define MINOR_SCREEN_WND_OPEN_KEEP      0x229   //窗口打开保持
#define MINOR_SCREEN_WALL_MIRROR        0x22a   //电视墙区域镜像
#define MINOR_SCREEN_UPLOAD_BASEMAP     0x22b   //上传底图
#define MINOR_SCREEN_SHOW_BASEMAP       0x22c   //显示底图
#define MINOR_SCREEN_HIDE_BASEMAP       0x22d   //隐藏底图
#define MINOR_SCREEN_MODIFY_SERIAL      0x22e   //配置串口参数

#define MINOR_SCREEN_SET_INPUT          0x251    /*修改输入源*/
#define MINOR_SCREEN_SET_OUTPUT         0x252    /*修改输出通道*/
#define MINOR_SCREEN_SET_OSD            0x253    /*修改虚拟LED*/
#define MINOR_SCREEN_SET_LOGO           0x254    /*修改LOGO*/
#define MINOR_SCREEN_SET_LAYOUT         0x255    /*设置布局*/
#define MINOR_SCREEN_PICTUREPREVIEW     0x256    /*回显操作*/


//2012-06-14 CVCS2.0, 窗口设置等操作在V1.0， V1.1中已经有了，当时在设备日志中没有定义 
#define MINOR_SCREEN_GET_OSD            0x257   /*获取虚拟LED*/
#define MINOR_SCREEN_GET_LAYOUT            0x258   /*获取布局*/
#define MINOR_SCREEN_LAYOUT_CTRL        0x259   /*布局控制*/
#define MINOR_GET_ALL_VALID_WND            0x260    /*获取所有有效窗口*/
#define MINOR_GET_SIGNAL_WND            0x261    /*获取单个窗口信息*/
#define MINOR_WINDOW_CTRL                0x262    /*窗口控制*/
#define MINOR_GET_LAYOUT_LIST            0x263    /*获取布局列表*/
#define MINOR_LAYOUT_CTRL                0x264    /*布局控制*/
#define MINOR_SET_LAYOUT                0x265    /*设置布局*/
#define MINOR_GET_SIGNAL_LIST            0x266    /*获取输入信号源列表*/
#define MINOR_GET_PLAN_LIST                0x267    /*获取预案列表*/
#define MINOR_SET_PLAN                    0x268    /*修改预案*/
#define MINOR_CTRL_PLAN                    0x269    /*控制预案*/
#define MINOR_CTRL_SCREEN                0x270    /*屏幕控制*/
#define MINOR_ADD_NETSIG                0x271    /*添加信号源*/
#define MINOR_SET_NETSIG                0x272    /*修改信号源*/
#define MINOR_SET_DECBDCFG                0x273    /*设置解码板参数*/
#define MINOR_GET_DECBDCFG                0x274    /*获取解码板参数*/
#define MINOR_GET_DEVICE_STATUS            0x275    /*获取设备信息*/
#define MINOR_UPLOAD_PICTURE            0x276    /*底图上传*/
#define MINOR_SET_USERPWD                0x277    /*设置用户密码*/
#define MINOR_ADD_LAYOUT                0x278    /*添加布局*/
#define MINOR_DEL_LAYOUT                0x279    /*删除布局*/
#define MINOR_DEL_NETSIG                0x280    /*删除信号源*/
#define MINOR_ADD_PLAN                    0x281    /*添加预案*/
#define MINOR_DEL_PLAN                    0x282    /*删除预案*/
#define MINOR_GET_EXTERNAL_MATRIX_CFG    0x283    //获取外接矩阵配置
#define MINOR_SET_EXTERNAL_MATRIX_CFG    0x284    //设置外接矩阵配置
#define    MINOR_GET_USER_CFG                0x285    //获取用户配置
#define    MINOR_SET_USER_CFG                0x286    //设置用户配置
#define    MINOR_GET_DISPLAY_PANEL_LINK_CFG 0x287    //获取显示墙连接配置
#define    MINOR_SET_DISPLAY_PANEL_LINK_CFG 0x288    //设置显示墙连接配置

#define    MINOR_GET_WALLSCENE_PARAM        0x289        //获取电视墙场景
#define    MINOR_SET_WALLSCENE_PARAM        0x28a        //设置电视墙场景
#define    MINOR_GET_CURRENT_WALLSCENE        0x28b        //获取当前使用场景
#define    MINOR_SWITCH_WALLSCENE            0x28c        //场景切换
#define    MINOR_SIP_LOGIN                    0x28d        //SIP注册成功
#define MINOR_VOIP_START                0x28e        //VOIP对讲开始
#define MINOR_VOIP_STOP                    0x28f        //VOIP对讲停止
#define MINOR_WIN_TOP                   0x290       //电视墙窗口置顶
#define MINOR_WIN_BOTTOM                0x291       //电视墙窗口置底

// Netra 2.2.2
#define MINOR_LOCAL_LOAD_HDISK          0x300            //本地加载硬盘             
#define MINOR_LOCAL_DELETE_HDISK        0x301            //本地删除异常不存在的硬盘

//KY2013 3.0.0
#define MINOR_LOCAL_MAIN_AUXILIARY_PORT_SWITCH    0X302 //本地主辅口切换
#define MINOR_LOCAL_HARD_DISK_CHECK                0x303 //本地物理硬盘自检

//Netra3.1.0
#define MINOR_LOCAL_CFG_DEVICE_TYPE        0x310    //本地配置设备类型
#define MINOR_REMOTE_CFG_DEVICE_TYPE    0x311    //远程配置设备类型
#define MINOR_LOCAL_CFG_WORK_HOT_SERVER    0x312    //本地配置工作机热备服务器
#define MINOR_REMOTE_CFG_WORK_HOT_SERVER 0x313    //远程配置工作机热备服务器
#define MINOR_LOCAL_DELETE_WORK            0x314    //本地删除工作机
#define MINOR_REMOTE_DELETE_WORK        0x315    //远程删除工作机
#define    MINOR_LOCAL_ADD_WORK            0x316    //本地添加工作机
#define MINOR_REMOTE_ADD_WORK            0x317    //远程添加工作机
#define MINOR_LOCAL_IPCHEATMAP_OUTPUT   0x318            /* 本地导出热度图文件      */
#define MINOR_LOCAL_IPCHEATFLOW_OUTPUT  0x319          /* 本地导出热度流量文件      */
#define MINOR_REMOTE_SMS_SEND           0x350    /*远程发送短信*/
#define MINOR_LOCAL_SMS_SEND            0x351   /*本地发送短信*/
#define MINOR_ALARM_SMS_SEND            0x352    /*发送短信报警*/
#define MINOR_SMS_RECV                  0x353     /*接收短信*/
//（备注：0x350、0x351是指人工在GUI或IE控件上编辑并发送短信）
#define MINOR_LOCAL_SMS_SEARCH          0x354  /*本地搜索短信*/
#define MINOR_REMOTE_SMS_SEARCH         0x355   /*远程搜索短信*/
#define MINOR_LOCAL_SMS_READ            0x356   /*本地查看短信*/
#define MINOR_REMOTE_SMS_READ           0x357   /*远程查看短信*/
#define MINOR_REMOTE_DIAL_CONNECT       0x358   /*远程开启手动拨号*/
#define MINOR_REMOTE_DIAL_DISCONN       0x359   /*远程停止手动拨号*/
#define MINOR_LOCAL_WHITELIST_SET       0x35A   /*本地配置白名单*/
#define MINOR_REMOTE_WHITELIST_SET      0x35B   /*远程配置白名单*/
#define MINOR_LOCAL_DIAL_PARA_SET       0x35C   /*本地配置拨号参数*/
#define MINOR_REMOTE_DIAL_PARA_SET      0x35D   /*远程配置拨号参数*/
#define MINOR_LOCAL_DIAL_SCHEDULE_SET   0x35E   /*本地配置拨号计划*/
#define MINOR_REMOTE_DIAL_SCHEDULE_SET  0x35F   /*远程配置拨号计划*/
#define MINOR_PLAT_OPER                 0x360   /* 平台操作*/
#define MINOR_REMOTE_CFG_POE_WORK_MODE  0x361//远程设置POE工作模式
#define MINOR_LOCAL_CFG_POE_WORK_MODE   0x362//本地设置POE工作模式
#define MINOR_REMOTE_CFG_FACE_CONTRAST  0x363//远程设置人脸比对配置
#define MINOR_LOCAL_CFG_FACE_CONTRAST   0x364//本地设置人脸比对配置
#define MINOR_REMOTE_CFG_WHITELIST_FACE_CONTRAST   0x365//远程设置白名单人脸比对配置
#define MINOR_LOCAL_CFG_WHITELIST_FACE_CONTRAST    0x366//本地设置白名单人脸比对配置
#define MINOR_LOCAL_CHECK_TIME          0x367   //本地手动校时
#define MINOR_VCA_ONEKEY_EXPORT_PICTURE 0x368 //一键导出图片
#define MINOR_VCA_ONEKEY_DELETE_PICTURE 0x369 //一键删除图片
#define MINOR_VCA_ONEKEY_EXPORT_VIDEO   0x36a //一键导出录像
#define MINOR_VCA_ONEKEY_DELETE_VIDEO   0x36b //一键删除录像


//0x400-0x1000 门禁操作类型
#define MINOR_REMOTE_OPEN_DOOR          0x400   //远程开门
#define MINOR_REMOTE_CLOSE_DOOR         0x401   //远程关门(受控)
#define MINOR_REMOTE_ALWAYS_OPEN        0x402   //远程常开(自由)
#define MINOR_REMOTE_ALWAYS_CLOSE       0x403   //远程常关(禁用)
#define MINOR_REMOTE_CHECK_TIME         0x404   //远程手动校时
#define MINOR_NTP_CHECK_TIME            0x405   //NTP自动校时
#define MINOR_REMOTE_CLEAR_CARD         0x406   //远程清空卡号
#define MINOR_REMOTE_RESTORE_CFG        0x407   //远程恢复默认参数
#define MINOR_ALARMIN_ARM               0x408   //防区布防
#define MINOR_ALARMIN_DISARM            0x409   //防区撤防
#define MINOR_LOCAL_RESTORE_CFG         0x40a   //本地恢复默认参数 
#define MINOR_REMOTE_CAPTURE_PIC        0x40b  //远程抓拍
#define MINOR_MOD_NET_REPORT_CFG        0x40c   //修改网络中心参数配置
#define MINOR_MOD_GPRS_REPORT_PARAM     0x40d   //修改GPRS中心参数配置
#define MINOR_MOD_REPORT_GROUP_PARAM    0x40e   //修改中心组参数配置
#define    MINOR_UNLOCK_PASSWORD_OPEN_DOOR  0x40f  //解除码输入
#define MINOR_AUTO_RENUMBER              0x410  //自动重新编号
#define MINOR_AUTO_COMPLEMENT_NUMBER     0x411  //自动补充编号
#define MINOR_NORMAL_CFGFILE_INPUT          0x412   //导入普通配置文件
#define MINOR_NORMAL_CFGFILE_OUTTPUT     0x413   //导出普通配置文件
#define MINOR_CARD_RIGHT_INPUT              0x414   //导入卡权限参数
#define MINOR_CARD_RIGHT_OUTTPUT          0x415   //导出卡权限参数
#define MINOR_LOCAL_USB_UPGRADE             0x416   //本地U盘升级
#define MINOR_REMOTE_VISITOR_CALL_LADDER     0x417  //访客呼梯
#define MINOR_REMOTE_HOUSEHOLD_CALL_LADDER   0x418  //住户呼梯
#define MINOR_REMOTE_ACTUAL_GUARD            0x419  //远程实时布防
#define MINOR_REMOTE_ACTUAL_UNGUARD          0x41a  //远程实时撤防
#define MINOR_REMOTE_CONTROL_NOT_CODE_OPER_FAILED     0x41b   //遥控器未对码操作失败
#define MINOR_REMOTE_CONTROL_CLOSE_DOOR               0x41c   //遥控器关门
#define MINOR_REMOTE_CONTROL_OPEN_DOOR                0x41d   //遥控器开门
#define MINOR_REMOTE_CONTROL_ALWAYS_OPEN_DOOR         0x41e   //遥控器常开门

#define MINOR_OPERATION_CUSTOM1        0x900  //门禁自定义操作1
#define MINOR_OPERATION_CUSTOM2        0x901  //门禁自定义操作2
#define MINOR_OPERATION_CUSTOM3        0x902  //门禁自定义操作3
#define MINOR_OPERATION_CUSTOM4        0x903  //门禁自定义操作4
#define MINOR_OPERATION_CUSTOM5        0x904  //门禁自定义操作5
#define MINOR_OPERATION_CUSTOM6        0x905  //门禁自定义操作6
#define MINOR_OPERATION_CUSTOM7        0x906  //门禁自定义操作7
#define MINOR_OPERATION_CUSTOM8        0x907  //门禁自定义操作8
#define MINOR_OPERATION_CUSTOM9        0x908  //门禁自定义操作9
#define MINOR_OPERATION_CUSTOM10       0x909  //门禁自定义操作10
#define MINOR_OPERATION_CUSTOM11       0x90a  //门禁自定义操作11
#define MINOR_OPERATION_CUSTOM12       0x90b  //门禁自定义操作12
#define MINOR_OPERATION_CUSTOM13       0x90c  //门禁自定义操作13
#define MINOR_OPERATION_CUSTOM14       0x90d  //门禁自定义操作14
#define MINOR_OPERATION_CUSTOM15       0x90e  //门禁自定义操作15
#define MINOR_OPERATION_CUSTOM16       0x90f  //门禁自定义操作16
#define MINOR_OPERATION_CUSTOM17       0x910  //门禁自定义操作17
#define MINOR_OPERATION_CUSTOM18       0x911  //门禁自定义操作18
#define MINOR_OPERATION_CUSTOM19       0x912  //门禁自定义操作19
#define MINOR_OPERATION_CUSTOM20       0x913  //门禁自定义操作20
#define MINOR_OPERATION_CUSTOM21       0x914  //门禁自定义操作21
#define MINOR_OPERATION_CUSTOM22       0x915  //门禁自定义操作22
#define MINOR_OPERATION_CUSTOM23       0x916  //门禁自定义操作23
#define MINOR_OPERATION_CUSTOM24       0x917  //门禁自定义操作24
#define MINOR_OPERATION_CUSTOM25       0x918  //门禁自定义操作25
#define MINOR_OPERATION_CUSTOM26       0x919  //门禁自定义操作26
#define MINOR_OPERATION_CUSTOM27       0x91a  //门禁自定义操作27
#define MINOR_OPERATION_CUSTOM28       0x91b  //门禁自定义操作28
#define MINOR_OPERATION_CUSTOM29       0x91c  //门禁自定义操作29
#define MINOR_OPERATION_CUSTOM30       0x91d  //门禁自定义操作30
#define MINOR_OPERATION_CUSTOM31       0x91e  //门禁自定义操作31
#define MINOR_OPERATION_CUSTOM32       0x91f  //门禁自定义操作32
#define MINOR_OPERATION_CUSTOM33       0x920  //门禁自定义操作33
#define MINOR_OPERATION_CUSTOM34       0x921  //门禁自定义操作34
#define MINOR_OPERATION_CUSTOM35       0x922  //门禁自定义操作35
#define MINOR_OPERATION_CUSTOM36       0x923  //门禁自定义操作36
#define MINOR_OPERATION_CUSTOM37       0x924  //门禁自定义操作37
#define MINOR_OPERATION_CUSTOM38       0x925  //门禁自定义操作38
#define MINOR_OPERATION_CUSTOM39       0x926  //门禁自定义操作39
#define MINOR_OPERATION_CUSTOM40       0x927  //门禁自定义操作40
#define MINOR_OPERATION_CUSTOM41       0x928  //门禁自定义操作41
#define MINOR_OPERATION_CUSTOM42       0x929  //门禁自定义操作42
#define MINOR_OPERATION_CUSTOM43       0x92a  //门禁自定义操作43
#define MINOR_OPERATION_CUSTOM44       0x92b  //门禁自定义操作44
#define MINOR_OPERATION_CUSTOM45       0x92c  //门禁自定义操作45
#define MINOR_OPERATION_CUSTOM46       0x92d  //门禁自定义操作46
#define MINOR_OPERATION_CUSTOM47       0x92e  //门禁自定义操作47
#define MINOR_OPERATION_CUSTOM48       0x92f  //门禁自定义操作48
#define MINOR_OPERATION_CUSTOM49       0x930  //门禁自定义操作49
#define MINOR_OPERATION_CUSTOM50       0x931  //门禁自定义操作50
#define MINOR_OPERATION_CUSTOM51       0x932  //门禁自定义操作51
#define MINOR_OPERATION_CUSTOM52       0x933  //门禁自定义操作52
#define MINOR_OPERATION_CUSTOM53       0x934  //门禁自定义操作53
#define MINOR_OPERATION_CUSTOM54       0x935  //门禁自定义操作54
#define MINOR_OPERATION_CUSTOM55       0x936  //门禁自定义操作55
#define MINOR_OPERATION_CUSTOM56       0x937  //门禁自定义操作56
#define MINOR_OPERATION_CUSTOM57       0x938  //门禁自定义操作57
#define MINOR_OPERATION_CUSTOM58       0x939  //门禁自定义操作58
#define MINOR_OPERATION_CUSTOM59       0x93a  //门禁自定义操作59
#define MINOR_OPERATION_CUSTOM60       0x93b  //门禁自定义操作60
#define MINOR_OPERATION_CUSTOM61       0x93c  //门禁自定义操作61
#define MINOR_OPERATION_CUSTOM62       0x93d  //门禁自定义操作62
#define MINOR_OPERATION_CUSTOM63       0x93e  //门禁自定义操作63
#define MINOR_OPERATION_CUSTOM64       0x93f  //门禁自定义操作64

#define MINOR_SET_WIFI_PARAMETER               0x950    //设置WIFI配置参数
#define MINOR_EZVIZ_LOGIN                      0x951    //萤石云登陆
#define MINOR_EZVIZ_LOGINOUT                   0x952    //萤石云登出
#define MINOR_LOCK_ADD                         0x953    //智能锁添加
#define MINOR_LOCK_DELETE                      0x954    //智能锁删除
#define MINOR_LOCK_GET_STATUS                  0x955    //智能锁状态获取
#define MINOR_LOCK_SET_TMP_PASSWORD            0x956    //智能锁临时密码下发
#define MINOR_LOCK_SET_SILENT_MODE             0x957    //智能锁静音设置
#define MINOR_LOCK_SET_LATE_WARNING            0x958    //智能锁晚归提醒设置
#define MINOR_LOCK_IPC_ADD                     0x959    //智能锁IPC关联
#define MINOR_LOCK_IPC_REMOVE                  0x95a    //智能锁IPC解除关联
#define MINOR_LOCK_DETECTOR_ADD                0x95b    //智能锁探测器关联
#define MINOR_LOCK_DETECTOR_REMOVE             0x95c    //智能锁探测器解除关联
#define MINOR_LOCK_MESSAGE_REMINDING_OPEN      0x95d    //智能锁消息提醒打开
#define MINOR_LOCK_MESSAGE_REMINDING_CLOSE     0x95e    //智能锁消息提醒关闭
#define MINOR_LOCK_SET_HEART_BEAT              0x95f    //智能锁心跳设置
#define MINOR_LOCK_REBOOT                      0x960        //智能锁重启
#define MINOR_LOCK_CLEAR_USER                  0x961        //智能锁清空用户
#define MINOR_LOCK_FORMAT                      0x962        //智能锁格式化
#define MINOR_LOCK_FINGER_CHANGE               0x963        //智能锁指纹改动
#define MINOR_LOCK_PASSWORD_CHANGE             0x964     //智能锁密码改动
#define MINOR_LOCK_CARD_CHANGE                 0x965     //智能锁卡信息改动
#define MINOR_LOCK_USER_CHANGE                 0x966      //智能锁用户信息改动
#define MINOR_LOCK_SYSTEM_CHANGE               0x967     //智能锁系统信息改动
#define MINOR_LOCK_CHANGE_ADD_UESR             0x968      //智能锁新增用户
#define MINOR_LOCK_CHANGE_DEL_UESR             0x969     //智能锁删除用户
#define MINOR_LOCK_CHANGE_CUSTOM_USER_NAME     0x96a    //智能锁自定义用户用户名改动
#define MINOR_LOCK_CHANGE_REMOTE_DEVICE        0x96b  //智能锁遥控器信息改动
#define MINOR_LOCK_CHANGE_ADD_FP               0x96c  //智能锁新增指纹
#define MINOR_LOCK_CHANGE_DEL_FP               0x96d  //智能锁删除指纹
#define MINOR_LOCK_CHANGE_ADD_PASSWORD         0x96e  //智能锁新增密码
#define MINOR_LOCK_CHANGE_DEL_PASSWORD         0x96f  //智能锁删除密码
#define MINOR_LOCK_CHANGE_ADD_CARD             0x970  //智能锁新增卡片
#define MINOR_LOCK_CHANGE_DEL_CARD             0x971  //智能锁删除卡片
#define MINOR_LOCK_NETWORK_SWITCH              0x972  //智能锁网络功能开关改动
#define MINOR_LOCK_CLEAR_NETWORK_DATA          0x973  //智能锁网络数据清空
#define MINOR_LOCK_CLEAR_HOST_USER             0x974  //智能锁清空主人用户
#define MINOR_LOCK_CLEAR_GUEST_USER            0x975  //智能锁清空客人用户
#define MINOR_LOCK_CLEAN_ALL_REMOTE_DEVICE     0x976   //遥控器用户信息清空
#define MINOR_LOCK_CLEAN_NORMAL_USER_FINGRT    0x977    //智能锁清空普通用户指纹
#define MINOR_LOCK_CLEAN_ALL_CARD              0x978      //智能锁清空所有卡片
#define MINOR_LOCK_CLEAN_ALL_PASSWORD          0x979    //智能锁清空所有密码
#define MINOR_START_WIRELESSSERVER             0x97a    //开启设备热点
#define MINOR_STOP_WIRELESSSERVER              0x97b    //关闭设备热点
#define MINOR_EMERGENCY_CARD_AUTH_NORMAL_CARD  0x97c    //应急管理卡授权普通卡
#define MINOR_CHANGE_ALWAYS_OPEN_RIGHT         0x97d    //通道模式改动
#define MINOR_LOCK_DOOR_BELL_EVENT             0x97e    //门铃事件（操作锁触发）

//2018-04-23 通用物联网关操作日志类型
#define    MINOR_ALARMHOST_GUARD     		0x1010    //普通布防(外出布防)
#define    MINOR_ALARMHOST_UNGUARD 		0x1011    //普通撤防
#define    MINOR_ALARMHOST_BYPASS 				0x1012    //旁路
#define    MINOR_ALARMHOST_DURESS_ACCESS 			0x1013    //挟持

#define    MINOR_ALARMHOST_RS485_PARAM               0x1018   //修改485配置参数
#define    MINOR_ALARMHOST_ALARM_OUTPUT 				0x1019    //控制触发器
#define    MINOR_ALARMHOST_ACCESS_OPEN 				0x101a    //控制门禁开
#define    MINOR_ALARMHOST_ACCESS_CLOSE 				0x101b    //控制门禁关
#define    MINOR_ALARMHOST_SIREN_OPEN 				0x101c    //控制警号开
#define    MINOR_ALARMHOST_SIREN_CLOSE 				0x101d    //控制警号关
#define    MINOR_ALARMHOST_MOD_ZONE_CONFIG 			0x101e    //修改防区参数
#define    MINOR_ALARMHOST_MOD_ALARMOUT_CONIFG 	0x101f    //修改触发器参数
#define    MINOR_ALARMHOST_MOD_ANALOG_CONFIG 		0x1020    //修改模拟量配置
#define    MINOR_ALARMHOST_RS485_CONFIG 				0x1021    //修改485通道配置
#define    MINOR_ALARMHOST_PHONE_CONFIG 				0x1022    //修改拨号配置
#define    MINOR_ALARMHOST_ADD_ADMIN 				0x1023    //增加管理员
#define    MINOR_ALARMHOST_MOD_ADMIN_PARAM 		0x1024    //修改管理员参数
#define    MINOR_ALARMHOST_DEL_ADMIN 					0x1025    //删除管理员
#define    MINOR_ALARMHOST_ADD_NETUSER 				0x1026    //增加后端操作员
#define    MINOR_ALARMHOST_MOD_NETUSER_PARAM 		0x1027    //修改后端操作员参数
#define    MINOR_ALARMHOST_DEL_NETUSER 				0x1028    //删除后端操作员
#define    MINOR_ALARMHOST_ADD_OPERATORUSER 		0x1029    //增加前端操作员
#define    MINOR_ALARMHOST_MOD_OPERATORUSER_PW 	0x102a    //修改前端操作员密码
#define    MINOR_ALARMHOST_DEL_OPERATORUSER 		0x102b    //删除前端操作员
#define    MINOR_ALARMHOST_ADD_KEYPADUSER 		0x102c    //增加键盘/读卡器用户	
#define    MINOR_ALARMHOST_DEL_KEYPADUSER 		0x102d    //删除键盘/读卡器用户	


#define    MINOR_ALARMHOST_MOD_HOST_CONFIG          0x1032    //修改主机配置
#define    MINOR_ALARMHOST_RESTORE_BYPASS 			0x1033    //旁路恢复

#define    MINOR_ALARMHOST_ALARMOUT_OPEN 			0x1034    //触发器开启
#define    MINOR_ALARMHOST_ALARMOUT_CLOSE 			0x1035    //触发器关闭
#define    MINOR_ALARMHOST_MOD_SUBSYSTEM_PARAM 	0x1036    //修改子系统参数配置
#define    MINOR_ALARMHOST_GROUP_BYPASS 				0x1037    //组旁路
#define    MINOR_ALARMHOST_RESTORE_GROUP_BYPASS 	0x1038    //组旁路恢复
#define    MINOR_ALARMHOST_MOD_GRPS_PARAM 			0x1039    //修改GPRS参数

#define    MINOR_ALARMHOST_MOD_REPORT_MOD 			0x103b    //修改上传方式配置
#define    MINOR_ALARMHOST_MOD_GATEWAY_PARAM 		0x103c    //修改门禁参数配置

#define    MINOR_STAY_ARM 	0x104c    //留守布防
#define    MINOR_QUICK_ARM 			0x104d    //即时布防
#define    MINOR_AUTOMATIC_ARM 		0x104e    //自动布防
#define    MINOR_AUTOMATIC_DISARM 		0x104f    //自动撤防
#define    MINOR_KEYSWITCH_ARM 	0x1050  //钥匙布撤防防区布防
#define    MINOR_KEYSWITCH_DISARM	0x1051  //钥匙布撤防防区撤防
#define    MINOR_CLEAR_ALARM	0x1052    //消警
#define    MINOR_MOD_FAULT_CFG		0x1053  //修改系统故障配置
#define    MINOR_MOD_EVENT_TRIGGER_ALARMOUT_CFG	0x1054  //修改事件触发触发器配置
#define    MINOR_SEARCH_EXTERNAL_MODULE  0x1055 //搜索外接模块
#define    MINOR_REGISTER_EXTERNAL_MODULE 0x1056  //重新注册外接模块
#define    MINOR_CLOSE_KEYBOARD_ALARM	0x1057 //关闭键盘报警提示音
#define    MINOR_MOD_3G_PARAM		0x1058  //修改3G参数
#define    MINOR_MOD_PRINT_PARAM 0x1059  //修改打印机参数
#define    MINOR_ALARMHOST_SD_CARD_FORMAT		0x1060    //SD卡格式化
#define MINOR_ALARMHOST_SUBSYSTEM_UPGRADE  0x1061 //子板固件升级

#define MINOR_PLAN_ARM_CFG     0x1062 //计划布撤防参数配置
#define MINOR_PHONE_ARM         0x1063 //手机布防
#define MINOR_PHONE_STAY_ARM     0x1064 //手机留守布防
#define MINOR_PHONE_QUICK_ARM   0x1065 //手机即时布防
#define MINOR_PHONE_DISARM     0x1066 //手机撤防
#define MINOR_PHONE_CLEAR_ALARM     0x1067 //手机消警
#define MINOR_WHITELIST_CFG     0x1068 //白名单配置
#define MINOR_TIME_TRIGGER_CFG    0x1069 //定时开关触发器配置
#define MINOR_CAPTRUE_CFG     0x106a //抓图参数配置
#define MINOR_TAMPER_CFG    0x106b //防区防拆参数配置

#define MINOR_REMOTE_KEYPAD_UPGRADE    0x106c //远程升级键盘
#define MINOR_ONETOUCH_AWAY_ARMING  0x106d //一键外出布防 
#define MINOR_ONETOUCH_STAY_ARMING     0x106e //一键留守布防
#define MINOR_SINGLE_PARTITION_ARMING_OR_DISARMING  0x106f //单防区布撤防 
#define MINOR_CARD_CONFIGURATION   0x1070 //卡参数配置
#define MINOR_CARD_ARMING_OR_DISARMING   0x1071 //刷卡布撤防
#define MINOR_EXPENDING_NETCENTER_CONFIGURATION    0x1072 //扩展网络中心配置
#define MINOR_NETCARD_CONFIGURATION    0x1073 //网卡配置
#define MINOR_DDNS_CONFIGURATION      0x1074 //DDNS配置
#define MINOR_RS485BUS_CONFIGURATION    0x1075 // 485总线参数配置
#define MINOR_RS485BUS_RE_REGISTRATION     0x1076 //485总线重新注册

#define MINOR_REMOTE_OPEN_ELECTRIC_LOCK 0x1077 //远程打开电锁
#define MINOR_REMOTE_CLOSE_ELECTRIC_LOCK 0x1078 //远程关闭电锁
#define MINOR_LOCAL_OPEN_ELECTRIC_LOCK 0x1079 //本地打开电锁
#define MINOR_LOCAL_CLOSE_ELECTRIC_LOCK 0x107a //本地关闭电锁
#define MINOR_OPEN_ALARM_LAMP      0x107b //打开警灯(远程)
#define MINOR_CLOSE_ALARM_LAMP      0x107c //关闭警灯(远程)


#define MINOR_TEMPORARY_PASSWORD   0x107d //临时密码操作记录

#define MINOR_HIDDNS_CONFIG     0x1082 // HIDDNS配置
#define MINOR_REMOTE_KEYBOARD_UPDATA   0x1083 //远程键盘升级日志
#define MINOR_ZONE_ADD_DETECTOR      0x1084 //防区添加探测器
#define MINOR_ZONE_DELETE_DETECTOR    0x1085 //防区删除探测器
#define MINOR_QUERY_DETECTOR_SIGNAL   0x1086 //主机查询探测器信号强度
#define MINOR_QUERY_DETECTOR_BATTERY  0x1087 //主机查询探测器电量
#define MINOR_SET_DETECTOR_GUARD    0x1088 //探测器布防
#define MINOR_SET_DETECTOR_UNGUARD   0x1089 //探测器撤防
#define MINOR_WIRELESS_CONFIGURATION     0x108a  //无线参数配置
#define MINOR_OPEN_VOICE        0x108b //打开语音
#define MINOR_CLOSE_VOICE       0x108c //关闭语音
#define MINOR_ENABLE_FUNCTION_KEY     0x108d //启用功能键
#define MINOR_DISABLE_FUNCTION_KEY      0x108e //关闭功能键
#define MINOR_READ_CARD    0x108f //巡更刷卡
#define MINOR_START_BROADCAST     0x1090 //打开语音广播
#define MINOR_STOP_BROADCAST   0x1091 //关闭语音广播
#define MINOR_REMOTE_ZONE_MODULE_UPGRADE   0x1092 //远程升级防区模块
#define MINOR_NETWORK_MODULE_EXTEND   0x1093 //网络模块参数配置
#define MINOR_ADD_CONTROLLER 	0x1094 //添加遥控器用户
#define MINOR_DELETE_CONTORLLER 	0x1095 //删除遥控器用户
#define MINOR_REMOTE_NETWORKMODULE_UPGRADE    0x1096 //远程升级网络模块
#define MINOR_WIRELESS_OUTPUT_ADD   0x1097 //注册无线输出模块
#define MINOR_WIRELESS_OUTPUT_DEL    0x1098 //删除无线输出模块
#define MINOR_WIRELESS_REPEATER_ADD   0x1099 //注册无线中继器
#define MINOR_WIRELESS_REPEATER_DEL    0x109a //删除无线中继器
#define MINOR_PHONELIST_CFG     0x109b //电话名单参数配置
#define MINOR_RF_SIGNAL_CHECK   0x109c // RF信号查询
#define MINOR_USB_UPGRADE   0x109d // USB升级
#define MINOR_DOOR_TIME_REMINDER_CFG 0x109f //门磁定时提醒参数配置
#define MINOR_WIRELESS_SIREN_ADD    0x1100 //注册无线警号
#define MINOR_WIRELESS_SIREN_DEL    0x1101 //删除无线警号
#define MINOR_OUT_SCALE_OPEN    0x1102 //辅电开启
#define MINOR_OUT_SCALE_CLOSE    0x1103 //辅电关闭

#define  MINOR_ALARMHOST_4G_MODULS_START  0x1108  //4G模块启用
#define  MINOR_ALARMHOST_4G_MODULS_STOP  0x1109  //4G模块停用

#define  MINOR_EZVIZ_CLOUD_START   0x1110   //萤石云启用
#define  MINOR_EZVIZ_CLOUD_STOP    0x1111   //萤石云停用
#define  MINOR_SIPUA_GRID_START    0x1112   //国网B启用
#define  MINOR_SIPUA_GRID_STOP    0x1113    //国网B停用

#define MINOR_MODBUS_FILE_DOWNLOAD   0x1114  //导出modbus协议配置文件
#define MINOR_MODBUS_FILE_UPLOAD      0x1115  //导入modbus协议配置文件

#define MINOR_RS485_DLL_FILE_DOWNLOAD   0x1116  //导出485协议库文件
#define MINOR_RS485_DLL_FLIE_UPLOAD   0x1117  //导入485协议库文件
#define MINOR_TX1_REBOOT              0x1118   //TX1系统正常重启



//2012-03-05 ITC操作日志类型
#define MINOR_SET_TRIGGERMODE_CFG          0x1001    /*设置触发模式参数*/
#define MINOR_GET_TRIGGERMODE_CFG          0x1002    /*获取触发模式参数*/
#define MINOR_SET_IOOUT_CFG                0x1003    /*设置IO输出参数*/
#define MINOR_GET_IOOUT_CFG                0x1004    /*获取IO输出参数*/
#define MINOR_GET_TRIGGERMODE_DEFAULT      0x1005    /*获取触发模式推荐参数*/
#define MINOR_GET_ITCSTATUS                0x1006    /*获取状态检测参数*/
#define MINOR_SET_STATUS_DETECT_CFG        0x1007    /*设置状态检测参数*/
#define MINOR_GET_STATUS_DETECT_CFG        0x1008    /*获取状态检测参数*/
#define MINOR_SET_VIDEO_TRIGGERMODE_CFG    0x1009  /*设置视频触发模式参数*/
#define MINOR_GET_VIDEO_TRIGGERMODE_CFG    0x100a   /*获取视频触发模式参数*/

//2013-04-19 ITS操作日志类型
#define MINOR_LOCAL_ADD_CAR_INFO            0x2001  /*本地添加车辆信息*/
#define MINOR_LOCAL_MOD_CAR_INFO            0x2002  /*本地修改车辆信息*/
#define MINOR_LOCAL_DEL_CAR_INFO            0x2003  /*本地删除车辆信息*/
#define MINOR_LOCAL_FIND_CAR_INFO           0x2004  /*本地查找车辆信息*/
#define MINOR_LOCAL_ADD_MONITOR_INFO        0x2005  /*本地添加布控信息*/
#define MINOR_LOCAL_MOD_MONITOR_INFO        0x2006  /*本地修改布控信息*/
#define MINOR_LOCAL_DEL_MONITOR_INFO        0x2007  /*本地删除布控信息*/
#define MINOR_LOCAL_FIND_MONITOR_INFO       0x2008  /*本地查询布控信息*/
#define MINOR_LOCAL_FIND_NORMAL_PASS_INFO   0x2009  /*本地查询正常通行信息*/
#define MINOR_LOCAL_FIND_ABNORMAL_PASS_INFO 0x200a  /*本地查询异常通行信息*/
#define MINOR_LOCAL_FIND_PEDESTRIAN_PASS_INFO   0x200b  /*本地查询正常通行信息*/
#define MINOR_LOCAL_PIC_PREVIEW             0x200c  /*本地图片预览*/
#define MINOR_LOCAL_SET_GATE_PARM_CFG       0x200d  /*设置本地配置出入口参数*/
#define MINOR_LOCAL_GET_GATE_PARM_CFG       0x200e  /*获取本地配置出入口参数*/
#define MINOR_LOCAL_SET_DATAUPLOAD_PARM_CFG 0x200f  /*设置本地配置数据上传参数*/
#define MINOR_LOCAL_GET_DATAUPLOAD_PARM_CFG 0x2010  /*获取本地配置数据上传参数*/

//2013-11-19新增日志类型
#define MINOR_LOCAL_DEVICE_CONTROL                         0x2011   /*本地设备控制(本地开关闸)*/
#define MINOR_LOCAL_ADD_EXTERNAL_DEVICE_INFO               0x2012   /*本地添加外接设备信息 */
#define MINOR_LOCAL_MOD_EXTERNAL_DEVICE_INFO               0x2013   /*本地修改外接设备信息 */
#define MINOR_LOCAL_DEL_EXTERNAL_DEVICE_INFO               0x2014   /*本地删除外接设备信息 */
#define MINOR_LOCAL_FIND_EXTERNAL_DEVICE_INFO              0x2015   /*本地查询外接设备信息 */
#define MINOR_LOCAL_ADD_CHARGE_RULE                        0x2016   /*本地添加收费规则 */
#define MINOR_LOCAL_MOD_CHARGE_RULE                        0x2017   /*本地修改收费规则 */
#define MINOR_LOCAL_DEL_CHARGE_RULE                        0x2018   /*本地删除收费规则 */
#define MINOR_LOCAL_FIND_CHARGE_RULE                       0x2019   /*本地查询收费规则 */
#define MINOR_LOCAL_COUNT_NORMAL_CURRENTINFO               0x2020   /*本地统计正常通行信息 */
#define MINOR_LOCAL_EXPORT_NORMAL_CURRENTINFO_REPORT       0x2021   /*本地导出正常通行信息统计报表 */
#define MINOR_LOCAL_COUNT_ABNORMAL_CURRENTINFO             0x2022   /*本地统计异常通行信息 */
#define MINOR_LOCAL_EXPORT_ABNORMAL_CURRENTINFO_REPORT     0x2023   /*本地导出异常通行信息统计报表 */
#define MINOR_LOCAL_COUNT_PEDESTRIAN_CURRENTINFO           0x2024   /*本地统计行人通行信息 */
#define MINOR_LOCAL_EXPORT_PEDESTRIAN_CURRENTINFO_REPORT   0x2025   /*本地导出行人通行信息统计报表 */
#define MINOR_LOCAL_FIND_CAR_CHARGEINFO                    0x2026   /*本地查询过车收费信息 */
#define MINOR_LOCAL_COUNT_CAR_CHARGEINFO                   0x2027   /*本地统计过车收费信息 */
#define MINOR_LOCAL_EXPORT_CAR_CHARGEINFO_REPORT           0x2028   /*本地导出过车收费信息统计报表 */
#define MINOR_LOCAL_FIND_SHIFTINFO                         0x2029   /*本地查询交接班信息 */
#define MINOR_LOCAL_FIND_CARDINFO                          0x2030   /*本地查询卡片信息 */
#define MINOR_LOCAL_ADD_RELIEF_RULE                        0x2031   /*本地添加减免规则 */
#define MINOR_LOCAL_MOD_RELIEF_RULE                        0x2032   /*本地修改减免规则 */
#define MINOR_LOCAL_DEL_RELIEF_RULE                        0x2033   /*本地删除减免规则 */
#define MINOR_LOCAL_FIND_RELIEF_RULE                       0x2034   /*本地查询减免规则 */
#define MINOR_LOCAL_GET_ENDETCFG                           0x2035   /*本地获取出入口控制机离线检测配置 */ 
#define MINOR_LOCAL_SET_ENDETCFG                           0x2036   /*本地设置出入口控制机离线检测配置*/
#define MINOR_LOCAL_SET_ENDEV_ISSUEDDATA                   0x2037   /*本地设置出入口控制机下发卡片信息 */
#define MINOR_LOCAL_DEL_ENDEV_ISSUEDDATA                   0x2038   /*本地清空出入口控制机下发卡片信息 */

#define MINOR_REMOTE_DEVICE_CONTROL                        0x2101   /*远程设备控制*/
#define MINOR_REMOTE_SET_GATE_PARM_CFG                     0x2102   /*设置远程配置出入口参数*/
#define MINOR_REMOTE_GET_GATE_PARM_CFG                     0x2103   /*获取远程配置出入口参数*/
#define MINOR_REMOTE_SET_DATAUPLOAD_PARM_CFG               0x2104   /*设置远程配置数据上传参数*/
#define MINOR_REMOTE_GET_DATAUPLOAD_PARM_CFG               0x2105   /*获取远程配置数据上传参数*/
#define MINOR_REMOTE_GET_BASE_INFO                         0x2106   /*远程获取终端基本信息*/
#define MINOR_REMOTE_GET_OVERLAP_CFG                       0x2107   /*远程获取字符叠加参数配置*/              
#define MINOR_REMOTE_SET_OVERLAP_CFG                       0x2108   /*远程设置字符叠加参数配置*/ 
#define MINOR_REMOTE_GET_ROAD_INFO                         0x2109   /*远程获取路口信息*/
#define MINOR_REMOTE_START_TRANSCHAN                       0x210a   /*远程建立同步数据服务器*/
#define MINOR_REMOTE_GET_ECTWORKSTATE                      0x210b   /*远程获取出入口终端工作状态*/
#define MINOR_REMOTE_GET_ECTCHANINFO                       0x210c   /*远程获取出入口终端通道状态*/

//远程控制 2013-11-19
#define MINOR_REMOTE_ADD_EXTERNAL_DEVICE_INFO              0x210d   /*远程添加外接设备信息 */
#define MINOR_REMOTE_MOD_EXTERNAL_DEVICE_INFO              0x210e   /*远程修改外接设备信息 */
#define MINOR_REMOTE_GET_ENDETCFG                          0x210f   /*远程获取出入口控制机离线检测配置 */ 
#define MINOR_REMOTE_SET_ENDETCFG                          0x2110   /*远程设置出入口控制机离线检测配置*/
#define MINOR_REMOTE_ENDEV_ISSUEDDATA                      0x2111   /*远程设置出入口控制机下发卡片信息 */
#define MINOR_REMOTE_DEL_ENDEV_ISSUEDDATA                  0x2112   /*远程清空出入口控制机下发卡片信息 */

//ITS 0x2115~0x2120 停车场车位项目
#define MINOR_REMOTE_ON_CTRL_LAMP           0x2115  /*开启远程控制车位指示灯*/       
#define MINOR_REMOTE_OFF_CTRL_LAMP          0x2116  /*关闭远程控制车位指示灯*/
//Netra3.1.0
#define MINOR_SET_VOICE_LEVEL_PARAM         0x2117  /*设置音量大小 */
#define MINOR_SET_VOICE_INTERCOM_PARAM      0x2118  /*设置音量录音 */
#define MINOR_SET_INTELLIGENT_PARAM         0x2119  /*智能配置*/ 
#define MINOR_LOCAL_SET_RAID_SPEED          0x211a  /*本地设置raid速度*/
#define MINOR_REMOTE_SET_RAID_SPEED         0x211b /*远程设置raid速度*/
//Nerta3.1.2
#define MINOR_REMOTE_CREATE_STORAGE_POOL    0x211c   //远程添加存储池
#define MINOR_REMOTE_DEL_STORAGE_POOL       0x211d    //远程删除存储池

#define MINOR_REMOTE_DEL_PIC                0x2120   //远程删除图片数据
#define MINOR_REMOTE_DEL_RECORD             0x2121   //远程删除录像数据
#define MINOR_REMOTE_CLOUD_ENABLE           0x2123  //远程设置云系统启用
#define MINOR_REMOTE_CLOUD_DISABLE          0x2124  //远程设置云系统禁用
#define MINOR_REMOTE_CLOUD_MODIFY_PARAM     0x2125  //远程修改存储池参数
#define MINOR_REMOTE_CLOUD_MODIFY_VOLUME    0x2126  //远程修改存储池容量
#define MINOR_REMOTE_GET_GB28181_SERVICE_PARAM    0x2127  //远程获取GB28181服务参数
#define MINOR_REMOTE_SET_GB28181_SERVICE_PARAM    0x2128  //远程设置GB28181服务参数
#define MINOR_LOCAL_GET_GB28181_SERVICE_PARAM     0x2129  //本地获取GB28181服务参数
#define MINOR_LOCAL_SET_GB28181_SERVICE_PARAM     0x212a  //本地配置B28181服务参数
#define MINOR_REMOTE_SET_SIP_SERVER               0x212b  //远程配置SIP SERVER
#define MINOR_LOCAL_SET_SIP_SERVER                0x212c  //本地配置SIP SERVER
#define MINOR_LOCAL_BLACKWHITEFILE_OUTPUT         0x212d  //本地黑白名单导出
#define MINOR_LOCAL_BLACKWHITEFILE_INPUT          0x212e  //本地黑白名单导入
#define MINOR_REMOTE_BALCKWHITECFGFILE_OUTPUT     0x212f  //远程黑白名单导出
#define MINOR_REMOTE_BALCKWHITECFGFILE_INPUT      0x2130  //远程黑白名单导入


#define MINOR_REMOTE_CREATE_MOD_VIEWLIB_SPACE        0x2200    /*远程创建/修改视图库空间*/
#define MINOR_REMOTE_DELETE_VIEWLIB_FILE            0x2201    /*远程删除视图库文件*/
#define MINOR_REMOTE_DOWNLOAD_VIEWLIB_FILE            0x2202    /*远程下载视图库文件*/
#define MINOR_REMOTE_UPLOAD_VIEWLIB_FILE            0x2203    /*远程上传视图库文件*/
#define MINOR_LOCAL_CREATE_MOD_VIEWLIB_SPACE        0x2204    /*本地创建/修改视图库空间*/

#define MINOR_LOCAL_SET_DEVICE_ACTIVE   0x3000  //本地激活设备
#define MINOR_REMOTE_SET_DEVICE_ACTIVE  0x3001  //远程激活设备
#define MINOR_LOCAL_PARA_FACTORY_DEFAULT    0x3002  //本地回复出厂设置
#define MINOR_REMOTE_PARA_FACTORY_DEFAULT   0x3003  //远程恢复出厂设置

/*信息发布服务器操作日志*/
#define MINOR_UPLAOD_STATIC_MATERIAL                0x2401  //静态素材上传
#define MINOR_UPLOAD_DYNAMIC_MATERIAL               0x2402  //动态素材上传
#define MINOR_DELETE_MATERIAL                       0x2403  //删除素材
#define MINOR_DOWNLOAD_STATIC_MATERIAL              0x2404  //静态素材下载
#define MINOR_COVER_STATIC_MATERIAL                 0x2405  //静态素材覆盖
#define MINOR_APPROVE_MATERIAL                      0x2406  //素材审核
#define MINOR_UPLAOD_PROGRAM                        0x2407  //上传节目
#define MINOR_DOWNLOAD_PROGRAM                      0x2408  //下载节目
#define MINOR_DELETE_PROGRAM                        0x2409  //删除节目
#define MINOR_MODIFY_PROGRAM                        0x240a  //节目属性修改
#define MINOR_APPROVE_PRAGRAM                       0x240b  //节目审核
#define MINOR_UPLAOD_SCHEDULE                       0x240c  //上传日程
#define MINOR_DOWNLOAD_SCHEDULE                     0x240d  //下载日程
#define MINOR_DELETE_SCHEDULE                       0x240e  //删除日程
#define MINOR_MODIFY_SCHEDULE                       0x240f  //修改日程属性
#define MINOR_RELEASE_SCHEDULE                      0x2410  //发布日程
#define MINOR_ADD_TERMINAL                          0x2411  //添加终端
#define MINOR_DELETE_TERMINAL                       0x2412  //删除终端
#define MINOR_MODIFY_TERMIANL_PARAM                 0x2413  //修改终端参数            
#define MINOR_MODIFY_TERMIANL_PLAY_PARAM            0x2414  //配置终端播放参数
#define MINOR_ADD_TERMIANL_GROUP                    0x2415  //添加终端组
#define MINOR_MODIFY_TERMINAL_GROUP_PARAM           0x2416  //修改终端组参数
#define MINOR_DELETE_TERMIANL_GROUP                 0x2417  //删除终端组
#define MINOR_TERMINAL_PLAY_CONTROL                 0x2418  //终端播放控制
#define MINOR_TERMINAL_ON_OFF_LINE                  0x2419  //终端上下线
#define MINOR_SET_SWITCH_PLAN                       0x241a  //设置终端定时开关机计划
#define MINOR_SET_VOLUME_PLAN                       0x241b  //设置终端定时音量计划
#define MINOR_TERMINAL_SCREENSHOT                   0x241c  //终端截屏
#define MINOR_SYSTEM_TIME_CFG                       0x241d   //系统校时
#define MINOR_ADD_USER_CFG                          0x241e    //添加用户配置
#define MINOR_DEL_USER_CFG                          0x241f    //删除用户配置
#define MINOR_REMOTE_MANAGE_HDD                     0x2420   //远程编辑硬盘
#define MINOR_TERMINAL_UPDATE_START                 0x2421   //终端升级
#define MINOR_SVR_RESTORE_DEFAULT_PARAM             0x2422   //服务器远程恢复默认
#define MINOR_SVR_REMOTE_RESTORE_FACTORY            0x2423   //服务器远程恢复出厂设置
#define MINOR_SVR_REMOTE_REBOOT                     0x2424   //服务器远程重启
#define MINOR_SVR_MODIFY_NETWORK_PARAM              0x2425   //服务器网络参数修改
#define MINOR_SVR_SOFTWARE_UPGRADE                  0x2426     //服务器软件升级

#define MINOR_BACKUP_DATA                           0xc41  //数据备份
#define MINOR_TRANSFER_DATA                         0xc42  //数据迁移
#define MINOR_RESTORE_DATA                          0xc43  //数据还原
#define MINOR_SET_INPUT_PLAN                        0xc44 //设置终端定时输入切换计划
#define MINOR_TERMINAL_ADB                          0xc45//终端ADB配置
#define MINOR_TERMINAL_VOLUME                       0xc46  //终端音量配置
#define MINOR_TERMINAL_LOGO                         0xc47 //终端LOGO配置
#define MINOR_TERMINAL_DEFAULT_SCHEDULE             0xc48 //垫片日程使能
#define MINOR_TERMINAL_PASSWORD                     0xc49 //设置终端密码
#define MINOR_TERMINAL_IP                           0xc4a//终端IP配置
#define MINOR_TERMINAL_RELATE_IPC                   0xc4b//终端关联IPC
#define MINOR_TERMINAL_SERVER                       0xc4c//终端关联服务器配置
#define MINOR_TERMINAL_SADP                         0xc4d//终端SADP开关配置
#define MINOR_TERMINAL_TIMEZONE                     0xc4e//终端时区配置
#define MINOR_TERMINAL_TEMP_PROTECT                 0xc4f//终端温度保护配置
#define MINOR_ADD_ORGANIZATION                      0xc50 //添加组织
#define MINOR_DELETE_ORGANIZATION                   0xc51 //删除组织
#define MINOR_MODIFY_ORGANIZATION                   0xc52 //修改组织 
#define MINOR_WEATHER_FACTORY                       0xc53 //天气厂商配置 
#define MINOR_SADP_ENABLE                           0xc54 //sadp开关配置 
#define MINOR_SSH_ENABLE                            0xc55 //SSH开关配置
#define MINOR_MODIFY_MATERIAL                       0xc56 //素材参数修改 
#define MINOR_INSERT_CHARACTER                      0xc57 //插播文字消息
#define MINOR_TERMINAL_BACKLIGHT                    0xc58 //终端背光配置



#define MINOR_REMOTE_CONFERENCE_CONFIG             0x2501  //MCU会议配置
#define MINOR_REMOTE_TERMINAL_CONFIG               0x2502  //MCU终端配置
#define MINOR_REMOTE_GROUP_CONFIG                  0x2503  //MCU分组配置
#define MINOR_REMOTE_CONFERENCE_CTRL               0x2504  //MCU会议控制
#define MINOR_REMOTE_TERMINAL_CTRL                 0x2505  //MCU终端控制

//NVR后端
#define MINOR_LOCAL_RESET_LOGIN_PASSWORD           0x2600    /* 本地重置admin登陆密码*/ 
#define MINOR_REMOTE_RESET_LOGIN_PASSWORD          0x2601    /* 远程重置admin登录密码 */
#define MINOR_LOCAL_FACE_BASE_CREATE        0x2602    /* 本地人脸对比库创建*/ 
#define MINOR_REMOTE_FACE_BASE_CREATE       0x2603    /* 远程人脸对比库创建*/
#define MINOR_LOCAL_FACE_BASE_MODIFY        0x2604    /* 本地人脸对比库修改*/ 
#define MINOR_REMOTE_FACE_BASE_MODIFY      0x2605    /* 远程人脸对比库修改*/
#define MINOR_LOCAL_FACE_BASE_DELETE        0x2606    /* 本地人脸对比库删除*/ 
#define MINOR_REMOTE_FACE_BASE_DELETE       0x2607    /* 远程人脸对比库删除*/
#define MINOR_LOCAL_FACE_DATA_APPEND        0x2608    /* 本地录入人脸数据*/ 
#define MINOR_REMOTE_FACE_DATA_APPEND       0x2609    /* 远程录入人脸数据*/
#define MINOR_LOCAL_FACE_DATA_SEARCH       0x2610    /* 本地人脸比对数据查找*/ 
#define MINOR_REMOTE_FACE_DATA_SEARCH       0x2611    /* 远程人脸比对数据查找*/
#define MINOR_LOCAL_FACE_DATA_ANALYSIS        0x2612    /* 本地图片分析操作*/ 
#define MINOR_REMOTE_FACE_DATA_ANALYSIS       0x2613    /* 远程图片分析操作*/
#define MINOR_LOCAL_FACE_DATA_EDIT            0x2614    /* 本地人脸数据修改*/ 
#define MINOR_REMOTE_FACE_DATA_EDIT           0x2615    /* 远程人脸数据修改*/

#define MINOR_LOCAL_FACE_DATA_DELETE          0x2616    /* 本地人脸数据删除*/ 
#define MINOR_REMOTE_FACE_DATA_DELET          0x2617    /* 远程人脸数据删除*/

#define MINOR_LOCAL_VCA_ANALYSIS_CFG          0x2618    /* 本地智能分析配置*/ 
#define MINOR_REMOTE_VCA_ANALYSIS_CFG         0x2619    /* 远程智能分析配置*/

#define MINOR_LOCAL_FACE_BASE_IMPORT          0x261a    /* 本地导入人脸库*/ 
#define MINOR_LOCAL_FACE_BASE_EXPORT          0x261b    /* 本地导出人脸库*/
//NVR集群
#define MINOR_REMOTE_CLUSTER_MODE_CONFIG      0x261c    /* 远程集群模式配置操作*/
#define MINOR_LOCAL_CLUSTER_MODE_CONFIG       0x261d    /* 本地集群模式配置操作*/
#define MINOR_REMOTE_CLUSTER_NETWORK_CONFIG   0x261e    /* 远程集群组网配置操作*/
#define MINOR_LOCAL_CLUSTER_NETWORK_CONFIG    0x261f    /* 本地集群组网配置操作*/
#define MINOR_REMOTE_CLUSTER_ADD_DEVICE       0x2620    /* 远程集群添加设备操作*/
#define MINOR_LOCAL_CLUSTER_ADD_DEVICE        0x2621    /* 本地集群添加设备操作*/
#define MINOR_REMOTE_CLUSTER_DEL_DEVICE       0x2622    /* 远程集群删除设备操作*/
#define MINOR_LOCAL_CLUSTER_DEL_DEVICE        0x2623    /* 本地集群删除设备操作*/
#define MINOR_REMOTE_HFPD_CFG                 0x2624  /* 远程高频人员检测配置*/
#define MINOR_REMOTE_FACE_CONTRAST_TASK       0x2625 /* 远程人脸比对任务配置 */

#define MINOR_REMOTE_IOTCFGFILE_INPUT         0x2627//远程导入IOT配置文件
#define MINOR_REMOTE_IOTCFGFILE_OUTPUT        0x2628//远程导出IOT配置文件
#define MINOR_LOCAL_IOT_ADD                   0x2629//本地添加IOT通道
#define MINOR_REMOTE_IOT_ADD                  0x262a//远程添加IOT通道
#define MINOR_LOCAL_IOT_DEL                   0x262b//本地删除IOT通道
#define MINOR_REMOTE_IOT_DEL                  0x262c//远程删除IOT通道
#define MINOR_LOCAL_IOT_SET                   0x262d//本地配置IOT通道
#define MINOR_REMOTE_IOT_SET                  0x262e//远程配置IOT通道
#define MINOR_LOCAL_IOTCFGFILE_INPUT          0x262f //本地导入IOT配置文件
#define MINOR_LOCAL_IOTCFGFILE_OUTPUT         0x2630//本地导出IOT配置文件
#define MINOR_LOCAL_VAD_CFG                   0x2631  /* 本地语音活动检测配置*/
#define MINOR_REMOTE_VAD_CFG                  0x2632  /* 远程语音活动检测配置*/
#define MINOR_LOCAL_ADDRESS_FILTER_CONFIG     0x2633    /* 本地地址过滤配置*/ 
#define MINOR_REMOTE_ADDRESS_FILTER_CONFIG    0x2634    /* 远程地址过滤配置*/
#define MINOR_LOCAL_POE_CFG                     0x2635    /* 本地POE配置*/ 
#define MINOR_REMOTE_POE_CFG                    0x2636    /* 远程POE配置*/
#define MINOR_LOCAL_RESET_CHANNEL_PASSWORD        0x2637    /* 本地重置通道密码*/ 
#define MINOR_REMOTE_RESET_CHANNEL_PASSWORD       0x2638    /* 远程重置通道密码*/


/*日志附加信息*/
//主类型
#define MAJOR_INFORMATION               0x4     /*附加信息*/
//次类型
#define MINOR_HDD_INFO                  0xa1 /*硬盘信息*/
#define MINOR_SMART_INFO                0xa2 /*SMART信息*/
#define MINOR_REC_START                 0xa3 /*开始录像*/
#define MINOR_REC_STOP                  0xa4 /*停止录像*/
#define MINOR_REC_OVERDUE                0xa5 /*过期录像删除*/
#define MINOR_LINK_START                0xa6 //连接前端设备
#define MINOR_LINK_STOP                    0xa7 //断开前端设备　
#define MINOR_NET_DISK_INFO                0xa8 //网络硬盘信息
#define MINOR_RAID_INFO                 0xa9 //raid相关信息
#define MINOR_RUN_STATUS_INFO           0xaa /*系统运行状态信息*/

//Netra3.0.0
#define MINOR_SPARE_START_BACKUP        0xab   /*热备系统开始备份指定工作机*/
#define MINOR_SPARE_STOP_BACKUP            0xac   /*热备系统停止备份指定工作机*/
#define MINOR_SPARE_CLIENT_INFO         0xad   /*热备客户机信息*/
#define MINOR_ANR_RECORD_START            0xae   /*ANR录像开始*/
#define MINOR_ANR_RECORD_END            0xaf   /*ANR录像结束*/
#define MINOR_ANR_ADD_TIME_QUANTUM        0xb0    /*ANR添加时间段*/
#define MINOR_ANR_DEL_TIME_QUANTUM        0xb1    /*ANR删除时间段*/

#define MINOR_PIC_REC_START             0xb3  /* 开始抓图*/    
#define MINOR_PIC_REC_STOP              0xb4  /* 停止抓图*/
#define MINOR_PIC_REC_OVERDUE           0xb5  /* 过期图片文件删除 */
//Netra3.1.0
#define  MINOR_CLIENT_LOGIN             0xb6   /*登录服务器成功*/
#define  MINOR_CLIENT_RELOGIN            0xb7   /*重新登录服务器*/
#define  MINOR_CLIENT_LOGOUT            0xb8   /*退出服务器成功*/
#define  MINOR_CLIENT_SYNC_START        0xb9   /*录像同步开始*/
#define  MINOR_CLIENT_SYNC_STOP            0xba   /*录像同步终止*/
#define  MINOR_CLIENT_SYNC_SUCC            0xbb   /*录像同步成功*/
#define  MINOR_CLIENT_SYNC_EXCP            0xbc   /*录像同步异常*/
#define  MINOR_GLOBAL_RECORD_ERR_INFO   0xbd   /*全局错误记录信息*/
#define  MINOR_BUFFER_STATE             0xbe   /*缓冲区状态日志记录*/
#define  MINOR_DISK_ERRORINFO_V2        0xbf   /*硬盘错误详细信息V2*/
#define  MINOR_CS_DATA_EXPIRED          0xc0   //云存储数据过期
#define  MINOR_PLAT_INFO                0xc1   //平台操作信息
#define  MINOR_DIAL_STAT                0xc2   /*拨号状态*/

#define MINOR_UNLOCK_RECORD             0xc3   //开锁记录
#define MINOR_VIS_ALARM                 0xc4   //防区报警
#define MINOR_TALK_RECORD               0xc5   //通话记录
#define MINOR_ACCESSORIES_MESSAGE       0xc6 //配件板信息
#define MINOR_KMS_EXPAMSION_DISK_INSERT 0xc7// KMS扩容盘插入
#define MINOR_IPC_CONNECT               0xc8//  IPC连接信息
#define MINOR_INTELLIGENT_BOARD_STATUS  0xc9//  智能板状态
#define MINOR_IPC_CONNECT_STATUS        0xca//  IPC连接状态
#define MINOR_AUTO_TIMING               0xcb  //自动校时
#define MINOR_EZVIZ_OPERATION           0xcc   //萤石运行状态
//NVR集群
#define MINOR_CLUSTER_DEVICE_ONLINE     0xcd   //集群设备上线
#define MINOR_CLUSTER_MGR_SERVICE_STARTUP 0xce   //集群管理服务启动
#define MINOR_CLUSTER_BUSINESS_TRANSFER 0xcf   //集群业务迁移
#define MINOR_CLUSTER_STATUS            0xd0   //集群状态信息
#define MINOR_CLUSTER_CS_STATUS            0xd1   //集群CS向CM发送设备状态失败，记录CS和CM的IP地址
#define MINOR_CLUSTER_CM_STATUS            0xd2   //CM状态切换，记录CM转变的角色，如leader、follower、candidate
#define MINOR_VOICE_START_DETECTED         0xd3  /*检测到语音开始*/
#define MINOR_VOICE_END_DETECTED           0xd4  /*检测到语音结束*/
#define MINOR_DOUBLE_VERIFICATION_PASS     0xd5  /*二次认证通过*/

/*事件*/
//主类型
#define MAJOR_EVENT                             0x5     /*事件*/
//次类型
#define MINOR_LEGAL_CARD_PASS                   0x01    //合法卡认证通过
#define MINOR_CARD_AND_PSW_PASS                 0x02    //刷卡加密码认证通过
#define MINOR_CARD_AND_PSW_FAIL                 0x03    //刷卡加密码认证失败
#define MINOR_CARD_AND_PSW_TIMEOUT              0x04    //数卡加密码认证超时
#define MINOR_CARD_AND_PSW_OVER_TIME            0x05    //刷卡加密码超次
#define MINOR_CARD_NO_RIGHT                     0x06    //未分配权限
#define MINOR_CARD_INVALID_PERIOD               0x07    //无效时段
#define MINOR_CARD_OUT_OF_DATE                  0x08    //卡号过期
#define MINOR_INVALID_CARD                      0x09    //无此卡号
#define MINOR_ANTI_SNEAK_FAIL                   0x0a    //反潜回认证失败
#define MINOR_INTERLOCK_DOOR_NOT_CLOSE          0x0b    //互锁门未关闭
#define MINOR_NOT_BELONG_MULTI_GROUP            0x0c    //卡不属于多重认证群组
#define MINOR_INVALID_MULTI_VERIFY_PERIOD       0x0d    //卡不在多重认证时间段内
#define MINOR_MULTI_VERIFY_SUPER_RIGHT_FAIL     0x0e    //多重认证模式超级权限认证失败
#define MINOR_MULTI_VERIFY_REMOTE_RIGHT_FAIL    0x0f    //多重认证模式远程认证失败
#define MINOR_MULTI_VERIFY_SUCCESS              0x10    //多重认证成功
#define MINOR_LEADER_CARD_OPEN_BEGIN            0x11    //首卡开门开始
#define MINOR_LEADER_CARD_OPEN_END              0x12    //首卡开门结束
#define MINOR_ALWAYS_OPEN_BEGIN                 0x13    //常开状态开始
#define MINOR_ALWAYS_OPEN_END                   0x14    //常开状态结束
#define MINOR_LOCK_OPEN                         0x15    //门锁打开
#define MINOR_LOCK_CLOSE                        0x16    //门锁关闭
#define MINOR_DOOR_BUTTON_PRESS                 0x17    //开门按钮打开
#define MINOR_DOOR_BUTTON_RELEASE               0x18    //开门按钮放开
#define MINOR_DOOR_OPEN_NORMAL                  0x19    //正常开门（门磁）
#define MINOR_DOOR_CLOSE_NORMAL                 0x1a    //正常关门（门磁）
#define MINOR_DOOR_OPEN_ABNORMAL                0x1b    //门异常打开（门磁）
#define MINOR_DOOR_OPEN_TIMEOUT                 0x1c    //门打开超时（门磁）
#define MINOR_ALARMOUT_ON                       0x1d    //报警输出打开
#define MINOR_ALARMOUT_OFF                      0x1e    //报警输出关闭
#define MINOR_ALWAYS_CLOSE_BEGIN                0x1f    //常关状态开始
#define MINOR_ALWAYS_CLOSE_END                  0x20    //常关状态结束                          
#define MINOR_MULTI_VERIFY_NEED_REMOTE_OPEN     0x21    //多重多重认证需要远程开门
#define MINOR_MULTI_VERIFY_SUPERPASSWD_VERIFY_SUCCESS  0x22  //多重认证超级密码认证成功事件
#define MINOR_MULTI_VERIFY_REPEAT_VERIFY        0x23    //多重认证重复认证事件
#define MINOR_MULTI_VERIFY_TIMEOUT               0x24    //多重认证重复认证事件
#define MINOR_DOORBELL_RINGING                  0x25    //门铃响
#define MINOR_FINGERPRINT_COMPARE_PASS          0x26    //指纹比对通过
#define MINOR_FINGERPRINT_COMPARE_FAIL          0x27    //指纹比对失败
#define MINOR_CARD_FINGERPRINT_VERIFY_PASS              0x28    //刷卡加指纹认证通过
#define MINOR_CARD_FINGERPRINT_VERIFY_FAIL              0x29    //刷卡加指纹认证失败
#define MINOR_CARD_FINGERPRINT_VERIFY_TIMEOUT           0x2a    //刷卡加指纹认证超时
#define MINOR_CARD_FINGERPRINT_PASSWD_VERIFY_PASS       0x2b    //刷卡加指纹加密码认证通过
#define MINOR_CARD_FINGERPRINT_PASSWD_VERIFY_FAIL       0x2c    //刷卡加指纹加密码认证失败
#define MINOR_CARD_FINGERPRINT_PASSWD_VERIFY_TIMEOUT    0x2d    //刷卡加指纹加密码认证超时
#define MINOR_FINGERPRINT_PASSWD_VERIFY_PASS            0x2e    //指纹加密码认证通过
#define MINOR_FINGERPRINT_PASSWD_VERIFY_FAIL            0x2f    //指纹加密码认证失败
#define MINOR_FINGERPRINT_PASSWD_VERIFY_TIMEOUT         0x30    //指纹加密码认证超时
#define MINOR_FINGERPRINT_INEXISTENCE                   0x31    //指纹不存在
#define MINOR_CARD_PLATFORM_VERIFY                      0x32    //刷卡平台认证
#define MINOR_CALL_CENTER                               0x33    //呼叫中心事件
#define MINOR_FIRE_RELAY_TURN_ON_DOOR_ALWAYS_OPEN       0x34    //消防继电器导通触发门常开
#define MINOR_FIRE_RELAY_RECOVER_DOOR_RECOVER_NORMAL    0x35   //消防继电器恢复门恢复正常
#define MINOR_FACE_AND_FP_VERIFY_PASS                   0x36    //人脸加指纹认证通过
#define MINOR_FACE_AND_FP_VERIFY_FAIL                   0x37    //人脸加指纹认证失败
#define MINOR_FACE_AND_FP_VERIFY_TIMEOUT                0x38    //人脸加指纹认证超时
#define MINOR_FACE_AND_PW_VERIFY_PASS                   0x39    //人脸加密码认证通过
#define MINOR_FACE_AND_PW_VERIFY_FAIL                   0x3a    //人脸加密码认证失败
#define MINOR_FACE_AND_PW_VERIFY_TIMEOUT                0x3b    //人脸加密码认证超时
#define MINOR_FACE_AND_CARD_VERIFY_PASS                 0x3c    //人脸加刷卡认证通过
#define MINOR_FACE_AND_CARD_VERIFY_FAIL                 0x3d    //人脸加刷卡认证失败
#define MINOR_FACE_AND_CARD_VERIFY_TIMEOUT              0x3e    //人脸加刷卡认证超时
#define MINOR_FACE_AND_PW_AND_FP_VERIFY_PASS            0x3f    //人脸加密码加指纹认证通过
#define MINOR_FACE_AND_PW_AND_FP_VERIFY_FAIL            0x40    //人脸加密码加指纹认证失败
#define MINOR_FACE_AND_PW_AND_FP_VERIFY_TIMEOUT         0x41    //人脸加密码加指纹认证超时
#define MINOR_FACE_CARD_AND_FP_VERIFY_PASS              0x42    //人脸加刷卡加指纹认证通过
#define MINOR_FACE_CARD_AND_FP_VERIFY_FAIL              0x43    //人脸加刷卡加指纹认证失败
#define MINOR_FACE_CARD_AND_FP_VERIFY_TIMEOUT           0x44    //人脸加刷卡加指纹认证超时
#define MINOR_EMPLOYEENO_AND_FP_VERIFY_PASS             0x45    //工号加指纹认证通过
#define MINOR_EMPLOYEENO_AND_FP_VERIFY_FAIL             0x46    //工号加指纹认证失败
#define MINOR_EMPLOYEENO_AND_FP_VERIFY_TIMEOUT          0x47    //工号加指纹认证超时
#define MINOR_EMPLOYEENO_AND_FP_AND_PW_VERIFY_PASS      0x48    //工号加指纹加密码认证通过
#define MINOR_EMPLOYEENO_AND_FP_AND_PW_VERIFY_FAIL      0x49    //工号加指纹加密码认证失败
#define MINOR_EMPLOYEENO_AND_FP_AND_PW_VERIFY_TIMEOUT   0x4a    //工号加指纹加密码认证超时
#define MINOR_FACE_VERIFY_PASS                          0x4b    //人脸认证通过
#define MINOR_FACE_VERIFY_FAIL                          0x4c    //人脸认证失败
#define MINOR_EMPLOYEENO_AND_FACE_VERIFY_PASS           0x4d    //工号加人脸认证通过
#define MINOR_EMPLOYEENO_AND_FACE_VERIFY_FAIL           0x4e    //工号加人脸认证失败
#define MINOR_EMPLOYEENO_AND_FACE_VERIFY_TIMEOUT        0x4f    //工号加人脸认证超时
#define MINOR_FACE_RECOGNIZE_FAIL                       0x50    //人脸识别失败
#define MINOR_FIRSTCARD_AUTHORIZE_BEGIN                    0x51    //首卡授权开始
#define MINOR_FIRSTCARD_AUTHORIZE_END                    0x52    //首卡授权结束
#define MINOR_DOORLOCK_INPUT_SHORT_CIRCUIT                0x53    //门锁输入短路报警
#define MINOR_DOORLOCK_INPUT_BROKEN_CIRCUIT                0x54    //门锁输入断路报警
#define MINOR_DOORLOCK_INPUT_EXCEPTION                    0x55    //门锁输入异常报警
#define MINOR_DOORCONTACT_INPUT_SHORT_CIRCUIT            0x56    //门磁输入短路报警
#define MINOR_DOORCONTACT_INPUT_BROKEN_CIRCUIT            0x57    //门磁输入断路报警
#define MINOR_DOORCONTACT_INPUT_EXCEPTION                0x58    //门磁输入异常报警
#define MINOR_OPENBUTTON_INPUT_SHORT_CIRCUIT            0x59    //开门按钮输入短路报警
#define MINOR_OPENBUTTON_INPUT_BROKEN_CIRCUIT            0x5a    //开门按钮输入断路报警
#define MINOR_OPENBUTTON_INPUT_EXCEPTION                0x5b    //开门按钮输入异常报警
#define MINOR_DOORLOCK_OPEN_EXCEPTION                    0x5c    //门锁异常打开
#define MINOR_DOORLOCK_OPEN_TIMEOUT                        0x5d    //门锁打开超时
#define MINOR_FIRSTCARD_OPEN_WITHOUT_AUTHORIZE            0x5e    //首卡未授权开门失败
#define MINOR_CALL_LADDER_RELAY_BREAK                   0x5f    //呼梯继电器断开
#define MINOR_CALL_LADDER_RELAY_CLOSE                   0x60    //呼梯继电器闭合
#define MINOR_AUTO_KEY_RELAY_BREAK                      0x61    //自动按键继电器断开
#define MINOR_AUTO_KEY_RELAY_CLOSE                      0x62    //自动按键继电器闭合
#define MINOR_KEY_CONTROL_RELAY_BREAK                   0x63    //按键梯控继电器断开
#define MINOR_KEY_CONTROL_RELAY_CLOSE                   0x64    //按键梯控继电器闭合
#define MINOR_EMPLOYEENO_AND_PW_PASS                    0x65    //工号加密码认证通过
#define MINOR_EMPLOYEENO_AND_PW_FAIL                    0x66    //工号加密码认证失败
#define MINOR_EMPLOYEENO_AND_PW_TIMEOUT                 0x67    //工号加密码认证超时
#define MINOR_HUMAN_DETECT_FAIL                         0x68    //真人检测失败
#define MINOR_PEOPLE_AND_ID_CARD_COMPARE_PASS           0x69    //人证比对通过
#define MINOR_PEOPLE_AND_ID_CARD_COMPARE_FAIL           0x70    //人证比对失败
#define MINOR_CERTIFICATE_BLACK_LIST                    0x71    //黑名单事件
#define MINOR_LEGAL_MESSAGE                             0x72    //合法短信
#define MINOR_ILLEGAL_MESSAGE                           0x73    //非法短信
#define MINOR_MAC_DETECT                                0x74    //MAC侦测
#define MINOR_DOOR_OPEN_OR_DORMANT_FAIL                 0x75   //门状态常闭或休眠状态认证失败
#define MINOR_AUTH_PLAN_DORMANT_FAIL                    0x76   //认证计划休眠模式认证失败
#define MINOR_CARD_ENCRYPT_VERIFY_FAIL                  0x77   //卡加密校验失败
#define MINOR_SUBMARINEBACK_REPLY_FAIL                  0x78   //反潜回服务器应答失败
#define MINOR_DOOR_OPEN_OR_DORMANT_OPEN_FAIL            0x82   //门常闭或休眠时开门按钮开门失败
#define MINOR_HEART_BEAT                                0x83    //心跳事件
#define MINOR_DOOR_OPEN_OR_DORMANT_LINKAGE_OPEN_FAIL    0x84   //门常闭或休眠时开门联动开门失败
#define MINOR_TRAILING                                  0x85   //尾随通行
#define MINOR_REVERSE_ACCESS                            0x86   //反向闯入
#define MINOR_FORCE_ACCESS                              0x87   //外力冲撞
#define MINOR_CLIMBING_OVER_GATE                        0x88   //翻越
#define MINOR_PASSING_TIMEOUT                           0x89   //通行超时
#define MINOR_INTRUSION_ALARM                           0x8a   //误闯报警
#define MINOR_FREE_GATE_PASS_NOT_AUTH                   0x8b   //闸机自由通行时未认证通过
#define MINOR_DROP_ARM_BLOCK                            0x8c   //摆臂被阻挡
#define MINOR_DROP_ARM_BLOCK_RESUME                     0x8d   //摆臂阻挡消除
#define MINOR_LOCAL_FACE_MODELING_FAIL                  0x8e   //设备升级本地人脸建模失败
#define MINOR_STAY_EVENT                                0x8f   //逗留事件
#define MINOR_PASSWORD_MISMATCH                         0x97   //密码不匹配
#define MINOR_EMPLOYEE_NO_NOT_EXIST                     0x98   //工号不存在
#define MINOR_COMBINED_VERIFY_PASS                      0x99   //组合认证通过
#define MINOR_COMBINED_VERIFY_TIMEOUT                   0x9a   //组合认证超时
#define MINOR_VERIFY_MODE_MISMATCH                      0x9b   //认证方式不匹配

#define MINOR_PASSPORT_VERIFY_FAIL                       0xa1   //护照信息校验失败

#define MINOR_EVENT_CUSTOM1                         0x500  //门禁自定义事件1
#define MINOR_EVENT_CUSTOM2                         0x501  //门禁自定义事件2
#define MINOR_EVENT_CUSTOM3                         0x502  //门禁自定义事件3
#define MINOR_EVENT_CUSTOM4                         0x503  //门禁自定义事件4
#define MINOR_EVENT_CUSTOM5                         0x504  //门禁自定义事件5
#define MINOR_EVENT_CUSTOM6                         0x505  //门禁自定义事件6
#define MINOR_EVENT_CUSTOM7                         0x506  //门禁自定义事件7
#define MINOR_EVENT_CUSTOM8                         0x507  //门禁自定义事件8
#define MINOR_EVENT_CUSTOM9                         0x508  //门禁自定义事件9
#define MINOR_EVENT_CUSTOM10                        0x509  //门禁自定义事件10
#define MINOR_EVENT_CUSTOM11                        0x50a  //门禁自定义事件11
#define MINOR_EVENT_CUSTOM12                        0x50b  //门禁自定义事件12
#define MINOR_EVENT_CUSTOM13                        0x50c  //门禁自定义事件13
#define MINOR_EVENT_CUSTOM14                        0x50d  //门禁自定义事件14
#define MINOR_EVENT_CUSTOM15                        0x50e  //门禁自定义事件15
#define MINOR_EVENT_CUSTOM16                        0x50f  //门禁自定义事件16
#define MINOR_EVENT_CUSTOM17                        0x510  //门禁自定义事件17
#define MINOR_EVENT_CUSTOM18                        0x511  //门禁自定义事件18
#define MINOR_EVENT_CUSTOM19                        0x512  //门禁自定义事件19
#define MINOR_EVENT_CUSTOM20                        0x513  //门禁自定义事件20
#define MINOR_EVENT_CUSTOM21                        0x514  //门禁自定义事件21
#define MINOR_EVENT_CUSTOM22                        0x515  //门禁自定义事件22
#define MINOR_EVENT_CUSTOM23                        0x516  //门禁自定义事件23
#define MINOR_EVENT_CUSTOM24                        0x517  //门禁自定义事件24
#define MINOR_EVENT_CUSTOM25                        0x518  //门禁自定义事件25
#define MINOR_EVENT_CUSTOM26                        0x519  //门禁自定义事件26
#define MINOR_EVENT_CUSTOM27                        0x51a  //门禁自定义事件27
#define MINOR_EVENT_CUSTOM28                        0x51b  //门禁自定义事件28
#define MINOR_EVENT_CUSTOM29                        0x51c  //门禁自定义事件29
#define MINOR_EVENT_CUSTOM30                        0x51d  //门禁自定义事件30
#define MINOR_EVENT_CUSTOM31                        0x51e  //门禁自定义事件31
#define MINOR_EVENT_CUSTOM32                        0x51f  //门禁自定义事件32
#define MINOR_EVENT_CUSTOM33                        0x520  //门禁自定义事件33
#define MINOR_EVENT_CUSTOM34                        0x521  //门禁自定义事件34
#define MINOR_EVENT_CUSTOM35                        0x522  //门禁自定义事件35
#define MINOR_EVENT_CUSTOM36                        0x523  //门禁自定义事件36
#define MINOR_EVENT_CUSTOM37                        0x524  //门禁自定义事件37
#define MINOR_EVENT_CUSTOM38                        0x525  //门禁自定义事件38
#define MINOR_EVENT_CUSTOM39                        0x526  //门禁自定义事件39
#define MINOR_EVENT_CUSTOM40                        0x527  //门禁自定义事件40
#define MINOR_EVENT_CUSTOM41                        0x528  //门禁自定义事件41
#define MINOR_EVENT_CUSTOM42                        0x529  //门禁自定义事件42
#define MINOR_EVENT_CUSTOM43                        0x52a  //门禁自定义事件43
#define MINOR_EVENT_CUSTOM44                        0x52b  //门禁自定义事件44
#define MINOR_EVENT_CUSTOM45                        0x52c  //门禁自定义事件45
#define MINOR_EVENT_CUSTOM46                        0x52d  //门禁自定义事件46
#define MINOR_EVENT_CUSTOM47                        0x52e  //门禁自定义事件47
#define MINOR_EVENT_CUSTOM48                        0x52f  //门禁自定义事件48
#define MINOR_EVENT_CUSTOM49                        0x530  //门禁自定义事件49
#define MINOR_EVENT_CUSTOM50                        0x531  //门禁自定义事件50
#define MINOR_EVENT_CUSTOM51                        0x532  //门禁自定义事件51
#define MINOR_EVENT_CUSTOM52                        0x533  //门禁自定义事件52
#define MINOR_EVENT_CUSTOM53                        0x534  //门禁自定义事件53
#define MINOR_EVENT_CUSTOM54                        0x535  //门禁自定义事件54
#define MINOR_EVENT_CUSTOM55                        0x536  //门禁自定义事件55
#define MINOR_EVENT_CUSTOM56                        0x537  //门禁自定义事件56
#define MINOR_EVENT_CUSTOM57                        0x538  //门禁自定义事件57
#define MINOR_EVENT_CUSTOM58                        0x539  //门禁自定义事件58
#define MINOR_EVENT_CUSTOM59                        0x53a  //门禁自定义事件59
#define MINOR_EVENT_CUSTOM60                        0x53b  //门禁自定义事件60
#define MINOR_EVENT_CUSTOM61                        0x53c  //门禁自定义事件61
#define MINOR_EVENT_CUSTOM62                        0x53d  //门禁自定义事件62
#define MINOR_EVENT_CUSTOM63                        0x53e  //门禁自定义事件63
#define MINOR_EVENT_CUSTOM64                        0x53f  //门禁自定义事件64

#define MINOR_LOCK_FINGER_OPEN_DOOR          0x600    //智能锁指纹开门
#define MINOR_LOCK_PASSWORD_OPEN_DOOR        0x601    //智能锁密码开门
#define MINOR_LOCK_CARD_OPEN_DOOR            0x602    //智能锁刷卡开门
#define MINOR_LOCK_CENTER_OPEN_DOOR          0x603    //智能锁中心开门
#define MINOR_LOCK_APP_OPEN_DOOR             0x604    //智能锁APP开门
#define MINOR_LOCK_KEY_OPEN_DOOR             0x605    //智能锁钥匙开门
#define MINOR_LOCK_REMOTE_DEVICE_OPEN_DOOR   0x606    //智能锁遥控器开门
#define MINOR_LOCK_TMP_PASSWORD_OPEN_DOOR    0x607    //智能锁临时密码开门
#define MINOR_LOCK_BLUETOOTH_OPEN_DOOR       0x608    //智能锁蓝牙开门
#define MINOR_LOCK_MULTI_OPEN_DOOR           0x609 //智能锁多重开门

//2018-04-23 通用物联网关事件日志类型
#define   MINOR_ALARMHOST_SCHOOLTIME_IRGI_B   0x1001     //B码校时
#define   MINOR_ALARMHOST_SCHOOLTIME_SDK     0x1002     //SDK校时
#define   MINOR_ALARMHOST_SCHOOLTIME_SELFTEST    0x1003   //定制自检校时
#define  MINOR_ALARMHOST_SUBSYSTEM_ABNORMALINSERT   0x1004  //子板插入 
#define  MINOR_ALARMHOST_SUBSYSTEM_ABNORMALPULLOUT   0x1005  //子板拔出

#define  MINOR_ALARMHOST_AUTO_ARM    0x1006  //自动布防
#define  MINOR_ALARMHOST_AUTO_DISARM    0x1007  //自动撤防
#define  MINOR_ALARMHOST_TIME_TIGGER_ON    0x1008  //定时开启触发器
#define  MINOR_ALARMHOST_TIME_TIGGER_OFF   0x1009  //定时关闭触发器
#define  MINOR_ALARMHOST_AUTO_ARM_FAILD    0x100a  //自动布防失败
#define  MINOR_ALARMHOST_AUTO_DISARM_FAILD    0x100b  //自动撤防失败
#define  MINOR_ALARMHOST_TIME_TIGGER_ON_FAILD   0x100c  //定时开启触发器失败
#define  MINOR_ALARMHOST_TIME_TIGGER_OFF_FAILD    0x100d  //定时关闭触发器失败
#define  MINOR_ALARMHOST_MANDATORY_ALARM    0x100e  //强制布防
#define  MINOR_ALARMHOST_KEYPAD_LOCKED  0x100f  //键盘锁定
#define  MINOR_ALARMHOST_USB_INSERT   0x1010  //USB插入 
#define  MINOR_ALARMHOST_USB_PULLOUT   0x1011 //USB拔出
#define  MINOR_ALARMHOST_4G_MODULS_ONLINE  0x1012  //4G模块上线
#define  MINOR_ALARMHOST_4G_MODULS_OFFLINE  0x1013  //4G模块下线

#define  MINOR_EZVIZ_CLOUD_ONLINE   0x1014   //萤石云上线
#define  MINOR_EZVIZ_CLOUD_OFFLINE    0x1015   //萤石云下线

#define  MINOR_SIPUA_GRID_ONLINE     0x1016   //国网B上线
#define  MINOR_SIPUA_GRID_OFFLINE    0x1017   //国网B下线

#define  MINOR_INTERNET_ACCESS_CONNECTED  0x1018   //网口连接
#define  MINOR_INTERNET_ACCESS_BREAK  0x1019  //网口断开

#define  MINOR_WIRELESS_CONNECTED   0x101a  //无线连接
#define  MINOR_WIRELESS_BREAK     0x101b   //无线断开


#endif

#endif //_EHOMEDEMO_LOGTYPE_H_