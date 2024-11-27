#ifndef WS2812_H
#define WS2812_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "led_strip.h"

#include <string.h>
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include <sys/socket.h>

#include "WIFI_STA.h"
#include "RMT_WS2812.h"


#include <time.h>
#include <sys/time.h>
#include "esp_attr.h"
#include "esp_netif_sntp.h"
#include "lwip/ip_addr.h"
#include "esp_sntp.h"

#define TimeColor               DeepSkyBlue
#define WeekColor_0             SeaGreen
#define WeekColor_1             Khaki

#define LightPink               0xFFB6C1 //浅粉红
#define Pink                    0xFFCOCB //粉红 
#define Crimson                 0xDC143C //猩红
#define LavenderBlush           0xFFFOF5 //脸红的淡紫色
#define PaleVioletRed           0xDB7093 //苍白的紫罗兰红色
#define HotPink                 0xFF69B4 //热情的粉红
#define DeepPink                0xFF1493 //深粉色
#define MediumVioletRed         0xC71585 //适中的紫罗兰红色
#define Orchid                  0xDA70D6 //兰花的紫色
#define Thistle                 0xD8BFD8 //蓟
#define plum                    0xDDAODD //李子
#define Violet                  0xEE82EE //紫罗兰
#define Magenta                 0xFFOOFF //洋红
#define Fuchsia                 0xFFOOFF //灯笼海棠(紫红色)
#define DarkMagenta             0x8B008B //深洋红色
#define Purple                  0x800080 //紫色
#define MediumOrchid            0xBA55D3 //适中的兰花紫
#define DarkVoilet              0x9400D3 //深紫罗兰色 
#define DarkOrchid              0x9932CC //深兰花紫
#define Indigo                  0x4B0082 //靛青 
#define Vtiole                  0x8A2BE2 //深紫罗兰的蓝色
#define MediumPurple            0x9370DB //适中的紫色
#define MediumSlateBlue         0x7B68EE //适中的板岩暗蓝灰色
#define SlateBlue               0x6A5ACD //板岩暗蓝灰色
#define DarkSlateBlue           0x483D8B //深岩暗蓝灰色
#define Lavender                0xE6E6FA //熏衣草花的淡紫色
#define GhostWhite              0xF8F8FF //幽灵的白色
#define Blue                    0x0000FF //纯蓝
    
    
#define MediumBlue              0x0000CD //适中的蓝色
#define MidnightBlue            0x191970 //午夜的蓝色
#define DarkBlue                0x00008B //深蓝色
#define Navy                    0x000080 //海军蓝
#define RoyalBlue               0x4169E1 //皇军蓝
#define CornflowerBlue          0x6495ED //矢车菊的蓝色
#define LightSteelBlue          0xBOC4DE //淡钢蓝
#define LightSlateGray          0x778899 //浅石板灰
#define SlateGray               0x708090 //石板灰
#define DoderBlue               0x1E90FF //道奇蓝
#define AliceBlue               0xFOF8FF //爱丽丝蓝
#define SteelBlue               0x4682B4 //钢蓝
#define LightSkyBlue            0x87CEFA //淡蓝色
#define SkyBlue                 0x87CEEB //天蓝色
#define DeepSkyBlue             0x00BFFF //深天蓝
#define LightBLue               0xADD8E6 //淡蓝
#define PowDerBlue              0xBOEOE6 //火药蓝
#define CadetBlue               0x5F9EAO //军校蓝
#define Azure                   0xFOFFFF //蔚蓝色 
#define LightCyan               0xE1FFFF //淡青色
#define PaleTurquoise           0xAFEEEE //苍白的绿宝石
#define Cyan                    0x00FFFF //青色 
#define Aqua                    0x00FFFF //水绿色
#define DarkTurquoise           0x00CED1 //深绿宝石 
#define DarkSlateGray           0x2F4F4F //深石板灰 
#define DarkCyan                0x008B8B //深青色 
#define Teal                    0x008080 //水鸭色
#define MediumTurquoise         0x48D1CC //适中的绿宝石
    
#define MediumAquamarine        0x00FA9A //适中的碧绿色
#define MediumSpringGreen       0xF5FFFA //适中的春天的绿色
#define MintCream               0x00FF7F //薄荷奶油
#define SpringGreen             0x3CB371 //春天的绿色
#define SeaGreen                0x2E8B57 //海洋绿
#define Honeydew                0xFOFFFO //蜂蜜
#define LightGreen              0x90EE90 //淡绿色
#define PaleGreen               0x98FB98 //苍白的绿色
#define DarkSeaGreen            0x8FBC8F //深海洋绿
#define LimeGreen               0x32CD32 //酸橙绿
#define Lime                    0x00FF00 //酸橙色
#define ForestGreen             0x228B22 //森林绿
#define Green                   0x008000 //纯绿
#define DarkGreen               0x006400 //深绿色
#define Chartreuse              0x7FFF00 //查特酒绿
#define LawnGreen               0x7CFC00 //草坪绿
#define GreenYellow             0xADFF2F //绿黄色
#define OliveDrab               0x556B2F //橄榄土褐色
#define Beige                   0x6B8E23 //米色(浅褐色)
#define LightGoldenrodYellow    0xFAFAD2 //浅秋麒麟黄
#define Ivory                   0xFFFFFO //象牙
#define LightYellow             0xFFFFEO //浅黄色
#define Yellow                  0xFFFFO0 //纯黄
#define Olive                   0x808000 //橄榄
#define DarkKhaki               0xBDB76B //深卡其布
#define LemonChiffon            0xFFFACD //柠檬薄纱
#define PaleGodenrod            0xEEE8AA //灰秋麒麟
#define Khaki                   0xF0E68C //卡其布

#define FloralWhite             0xFFFAFO //花的白色
#define OldLace                 0xFDF5E6 //老饰带
#define Wheat                   0xF5DEB3 //小麦色
#define Moccasin                0xFFE4B5 //鹿皮鞋
#define Orange                  0xFFA500 //橙色
#define PapayaWhip              0xFFEFD5 //番木瓜
#define BlanchedAlmond          0xFFEBCD //漂白的杏仁
#define NavajoWhite Navajo      0xFFDEAD //白
#define AntiqueWhite            0xFAEBD7 //古代的白色
#define Tan                     0xD2B48C //晒黑
#define BrulyWood               0xDEB887 //结实的树
#define Bisque                  0xFFE4C4 //(浓汤)乳脂,番茄等
#define DarkOrange              0xFF8C00 //深橙色
#define Linen                   0xFAFOE6 //亚麻布
#define Peru                    0xCD853F //秘鲁
#define PeachPuff               0xFFDAB9 //桃色
#define SandyBrown              0xF4A460 //沙棕色
#define Chocolate               0xD2691E //巧克力
#define SaddleBrown             0x8B4513 //马鞍棕色
#define SeaShel1                0xFFF5EE //海贝壳
#define Sienna                  0xA0522D //黄土赭色
#define LightSalmon             0xFFA07A //浅鲜肉(鲑鱼)色
#define Coral                   0xFF7F50 //珊瑚
#define OrangeRed               0xFF4500 //橙红色
#define DarkSalmon              0xE9967A //深鲜肉(鲑鱼)色
#define Tomato                  0xFF6347 //番茄
#define MistyRose               0xFFE4E1 //薄雾玫瑰
#define Salmon                  0xxFA8072 //鲜肉(鲑鱼)色

#define Snow                    0xFFFAFA //雪
#define LightCoral              0xF08080 //淡珊瑚色
#define RosyBrown               0xBC8F8F //玫瑰棕色


#endif