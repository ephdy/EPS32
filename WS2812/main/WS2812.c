#include "WS2812.h"


uint8_t buffer[1024];
uint8_t RGB[256*3];
uint8_t MODE=0;
bool led_on_off = true;
static const char *TAG="WS2812" ;
uint8_t brightness = 2; // 百分比





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
    inet_pton(AF_INET, "192.168.1.103", &server_addr.sin_addr); //将点分十进制的IP地址转换为二进制形式，并存储在server_addr.sin_addr中
 
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
                MODE=buffer[1];
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
