#include "WS2812.h"

uint8_t buffer[1024];
uint8_t RGB[256*3];
uint8_t MODE=1;
bool led_on_off = true;
static const char *TAG="WS2812" ;
uint8_t brightness = 20; // 百分比

struct Date_t
{
    uint8_t Hour;
    uint8_t Minute;
    uint8_t Second;
    uint8_t Day;
    uint8_t Month;
    uint8_t Year;
    uint8_t Week;
};
uint8_t Dates[256*3]={0};
struct Date_t Date={
    .Second=48,
    .Minute=36,
    .Hour=12,
    .Week=1,
};
//miN E1 EA F1
const uint8_t Code3x5[][15]={
    {1,1,1,1,1,
     1,0,0,0,1,
     1,1,1,1,1},//0
    {0,1,0,0,1,
     1,1,1,1,1,
     0,0,0,0,1},//1
    {1,0,1,1,1,
     1,0,1,0,1,
     1,1,1,0,1},//2
    {1,0,1,0,1,
     1,0,1,0,1,
     1,1,1,1,1},//3
    {1,1,1,0,0,
     0,0,1,0,0,
     1,1,1,1,1},//4
    {1,1,1,0,1,
     1,0,1,0,1,
     1,0,1,1,1},//5
    {1,1,1,1,1,
     1,0,1,0,1,
     1,0,1,1,1},//6
    {1,0,0,0,0,
     1,0,0,0,0,
     1,1,1,1,1},//7
    {1,1,1,1,1,
     1,0,1,0,1,
     1,1,1,1,1},//8
    {1,1,1,0,1,
     1,0,1,0,1,
     1,1,1,1,1},//9
    {0,0,0,0,0,
     0,1,0,1,0,
     0,0,0,0,0},//:
};

int create_socket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);//创建套接字
    //socket函数创建成功时的返回值是文件描述符，如果返回-1，则表示创建失败
    //socket三个参数分别是：地址类型（AF_INET表示IPv4）、套接字类型（SOCK_STREAM表示TCP）、协议（0表示默认协议）
    if (sockfd < 0) {//若返回值为-1，则表示创建失败，处理创建失败的情况
        perror("socket");//以socket：形式输出错误信息
        exit(EXIT_FAILURE);//退出
        return -1;
    }
    return sockfd;//返回创建成功的文件描述符
}

void connect_to_server(int sockfd) {
    struct sockaddr_in server_addr;//创建一个结构体变量，用于存储服务器的地址信息
    memset(&server_addr, 0, sizeof(server_addr));//清零初始化
    server_addr.sin_family = AF_INET;//设置地址族为IPv4
    server_addr.sin_port = htons(8080);//设置端口号为8080
    inet_pton(AF_INET, "192.168.1.104", &server_addr.sin_addr); //将点分十进制的IP地址转换为二进制形式，并存储在server_addr.sin_addr中
 
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {//连接服务器
        perror("connect");//以connect：形式输出错误信息
        closesocket(sockfd);//关闭套接字
        exit(EXIT_FAILURE);//退出
        vTaskDelay(6 / portTICK_PERIOD_MS);//延时6ms
    }
}

void Mode_0(led_strip_handle_t led_strip,int sockfd){
    switch (buffer[0])
    {
    case 0x00:
        led_on_off=false;send(sockfd, "WS2812_OFF", 10, 0);
        WS2812_Updata(led_strip,led_on_off,RGB,brightness);
        break;
    case 0x11:
        led_on_off=true;send(sockfd, "WS2812_ON", 9, 0);
        WS2812_Updata(led_strip,led_on_off,RGB,brightness);
        break;
    case 0x02:
        for (uint8_t i = 0; i < buffer[2]; i++)
        {
            RGB[(buffer[1]+i)*3]=buffer[3+i*3];
            RGB[(buffer[1]+i)*3+1]=buffer[3+i*3+1];
            RGB[(buffer[1]+i)*3+2]=buffer[3+i*3+2];
        }
        
        WS2812_Updata(led_strip,led_on_off,RGB,brightness);
        
        send(sockfd, RGB,256*3, 0);
        break;
    case 0x08:
        brightness=buffer[1];
        WS2812_Updata(led_strip,led_on_off,RGB,brightness);
        send(sockfd, "Brightness Change!",18 , 0);
        break;
    default:
        break;
    }
}

void Mode_1(led_strip_handle_t led_strip,int sockfd){
       /*
        for (uint8_t j = 0; j < 5; j++)
        {
            Dates[(0xe1+j)*3]=Code3x5[0][j]?0xff:0;
            Dates[(0xea+j)*3]=Code3x5[0][j+5]?0xff:0;
            Dates[(0xf1+j)*3]=Code3x5[0][j+10]?0xff:0;
        }
        */ 
    for (uint8_t j = 0; j < 5; j++)
    {
        Dates[(225+j)*3]=Code3x5[Date.Second%10][j]?0xff:0;
        Dates[(234+j)*3]=Code3x5[Date.Second%10][j+5]?0xff:0;
        Dates[(241+j)*3]=Code3x5[Date.Second%10][j+10]?0xff:0;

        Dates[(202+j)*3]=Code3x5[Date.Second/10][4-j]?0xff:0;
        Dates[(209+j)*3]=Code3x5[Date.Second/10][4-j+5]?0xff:0;
        Dates[(218+j)*3]=Code3x5[Date.Second/10][4-j+10]?0xff:0;


        Dates[(193+j)*3]=Code3x5[10][j+5]?0xff:0;

        Dates[(170+j)*3]=Code3x5[Date.Minute%10][4-j]?0xff:0;
        Dates[(177+j)*3]=Code3x5[Date.Minute%10][4-j+5]?0xff:0;
        Dates[(186+j)*3]=Code3x5[Date.Minute%10][4-j+10]?0xff:0;

        Dates[(145+j)*3]=Code3x5[Date.Minute/10][j]?0xff:0;
        Dates[(154+j)*3]=Code3x5[Date.Minute/10][j+5]?0xff:0;
        Dates[(161+j)*3]=Code3x5[Date.Minute/10][j+10]?0xff:0;

        Dates[(138+j)*3]=Code3x5[10][j+5]?0xff:0;

        Dates[(113+j)*3]=Code3x5[Date.Hour%10][j]?0xff:0;
        Dates[(122+j)*3]=Code3x5[Date.Hour%10][j+5]?0xff:0;
        Dates[(129+j)*3]=Code3x5[Date.Hour%10][j+10]?0xff:0;

        Dates[(90+j)*3]=Code3x5[Date.Hour/10][4-j]?0xff:0;
        Dates[(97+j)*3]=Code3x5[Date.Hour/10][4-j+5]?0xff:0;
        Dates[(106+j)*3]=Code3x5[Date.Hour/10][4-j+10]?0xff:0;
    }
    if (Date.Week==7){Dates[(88)*3+2]=0xff;Dates[(103)*3+2]=0xff;}
    else{Dates[(88)*3+1]=0xff;Dates[(103)*3+1]=0xff;}
    if (Date.Week==1){Dates[(119)*3+2]=0xff;Dates[(120)*3+2]=0xff;}
    else{Dates[(119)*3+1]=0xff;Dates[(120)*3+1]=0xff;}
    if (Date.Week==2){Dates[(136)*3+2]=0xff;Dates[(151)*3+2]=0xff;}
    else{Dates[(136)*3+1]=0xff;Dates[(151)*3+1]=0xff;}
    if (Date.Week==3){Dates[(167)*3+2]=0xff;Dates[(168)*3+2]=0xff;}
    else{Dates[(167)*3+1]=0xff;Dates[(168)*3+1]=0xff;}
    if (Date.Week==4){Dates[(184)*3+2]=0xff;Dates[(199)*3+2]=0xff;}
    else{Dates[(184)*3+1]=0xff;Dates[(199)*3+1]=0xff;}
    if (Date.Week==5){Dates[(215)*3+2]=0xff;Dates[(216)*3+2]=0xff;}
    else{Dates[(215)*3+1]=0xff;Dates[(216)*3+1]=0xff;}
    if (Date.Week==6){Dates[(232)*3+2]=0xff;Dates[(247)*3+2]=0xff;}
    else{Dates[(232)*3+1]=0xff;Dates[(247)*3+1]=0xff;}
    
    
   
    
    
    

    WS2812_Updata(led_strip,led_on_off,Dates,brightness);
    //send(sockfd, Dates,256*3, 0);
    uint8_t a=Date.Second%10;
    send(sockfd, &a,1, 0);
}


void app_main(void)
{
    ESP_LOGI(TAG,"ws2812 start");
    led_strip_handle_t led_strip = configure_led();

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    wifi_init_sta();

    int err;
    int sockfd = create_socket();//创建套接字
    connect_to_server(sockfd);//将套接字连接服务器
    vTaskDelay(1000 / portTICK_PERIOD_MS);//延时1s

    for (uint16_t i = 0; i < 256*3; i++)
    {
        RGB[i]=255;
    }
    
    WS2812_Updata(led_strip,1,RGB,brightness);
    err = send(sockfd, "hello ESP32!", 12, 0);
    Mode_1(led_strip,sockfd);
    while (1)
    {

        //led_on_off = !led_on_off;
        //vTaskDelay(pdMS_TO_TICKS(500));
        vTaskDelay(1000 / portTICK_PERIOD_MS);//延时1s

        err = recv(sockfd, buffer, sizeof(buffer), 0x08);//通过套接字接收服务器发送的网络数据
        if(err > 0){
            send(sockfd, "ESP32 Get!", 10, 0);
            switch (buffer[0])
            {
            case 0xEF:
                MODE=buffer[1];send(sockfd, "MODE CHANGE", 12, 0);
                break;
            default:
                switch (MODE)
                {
                case 0:
                    Mode_0(led_strip,sockfd);
                    break;
                case 1:
                    Mode_1(led_strip,sockfd);
                    break;
                default:
                    break;
                }
                break;
            }
            
            }

    }
    

}
