#include "timer.h"
#include "chan.h"

chan_t* chan;

void* __check__timeout(tju_tcp_t* sock) {

}

// 检查是否超时
int time_after(tju_tcp_t* sock) {
    // 这里我们通过通道进行异步监测时钟，倘若计时器到时而没有受到ACK
    // 则通过channel发送对应的信号量， 倘若收到对应的ACK则返回对应的信号量
    // 通道的使用同golang channel.
    chan = chan_init(0);
    int signal;
    pthread_t* thread;
    pthread_create(&thread, NULL, __check__timeout, (void*)sock);

    switch (chan_select(&chan, NULL, &signal, NULL, 0, NULL)) {
        case 0:
            if(signal == 1) {
                // timeout
            }else if(signal == 0) {
                // ack
            }
        default:
            printf("no received message.\n");
    }

    chan_dispose(chan);
}

void tcp_init_timer(
    tju_tcp_t* sock, 
    void (*retransmit_handler)(unsigned long)
) {
    tcp_init_rtt(sock);
    sock->rtt_timer->callback = retransmit_handler;
}

void tcp_init_rtt(struct tju_tcp_t* sock) {
    sock->rtt_timer = (rtt_timer_t*)malloc(sizeof(rtt_timer_t));
    sock->rtt_timer->estimated_rtt = 1;
    sock->rtt_timer->dev_rtt = 1;
    sock->rtt_timer->timeout = 1;
}

void tcp_set_estimator(tju_tcp_t* sock, float mrtt_us) {
    sock->rtt_timer->estimated_rtt = (1 - ALPHA) *  sock->rtt_timer->estimated_rtt + ALPHA * mrtt_us;
    sock->rtt_timer->dev_rtt = (1 - BETA) * sock->rtt_timer->dev_rtt + BETA * abs(sock->rtt_timer->estimated_rtt - mrtt_us);
}

void tcp_bound_rto(tju_tcp_t* sock) {
    if(sock->rtt_timer->timeout > TCP_RTO_MAX) {
        sock->rtt_timer->timeout = TCP_RTO_MAX;
    }
}

void tcp_set_rto(tju_tcp_t* sock) {
    sock->rtt_timer->timeout = sock->rtt_timer->estimated_rtt = max(CLOCK_G, 4 * sock->rtt_timer->dev_rtt);
    tcp_bound_rto(sock);
}

int tcp_ack_update_rtt(tju_tcp_t* sock, float seq_rtt_us, float sack_rtt_us) {

    /* Prefer RTT measured from ACK's timing to TS-ECR. This is because
	 * broken middle-boxes or peers may corrupt TS-ECR fields. But
	 * Karn's algorithm forbids taking RTT if some retransmitted data
	 * is acked (RFC6298).
	 */
	if (seq_rtt_us < 0)
		seq_rtt_us = sack_rtt_us;

    tcp_set_estimator(sock, seq_rtt_us);
    tcp_set_rto(sock);
    return 0;
}

// 当计时器超时时的回调函数
void tcp_write_timer_handler(tju_tcp_t* sock) {
    if(time_after(sock)) {
        return;
    }

    // 这里需要针对socket的状态进行不同的操作
    switch(sock->state) {
        default:
            printf("Error.\n");
    }
}


// 超时重传函数处理
// 检查当前socket状态
// 检查是否超出了重传次数
// 进入LOSS状态，开始慢启动
void tcp_retransmit_timer(tju_tcp_t* sock) {

}

// 慢启动
void tcp_entry_loss(tju_tcp_t* sock) {

}