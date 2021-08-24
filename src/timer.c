#include "timer.h"
#include "global.h"

// 检查是否超时
int time_after(tju_tcp_t* sock) {

}

void tcp_init_timer(
    tju_tcp_t* sock, 
    void (*retransmit_handler)(unsigned long)
) {
    tcp_init_rtt(sock);
    sock->rtt_timer.callback = retransmit_handler;
}

void tcp_init_rtt(struct tju_tcp_t* sock) {
    sock->rtt_timer.estimated_rtt = 1;
    sock->rtt_timer.dev_rtt = 1;
    sock->rtt_timer.timeout = 1;
}

void tcp_set_estimator(tju_tcp_t* sock, float mrtt_us) {
    sock->rtt_timer.estimated_rtt = (1 - ALPHA) *  sock->rtt_timer.estimated_rtt + ALPHA * mrtt_us;
    sock->rtt_timer.dev_rtt = (1 - BETA) * sock->rtt_timer.dev_rtt + BETA * abs(sock->rtt_timer.estimated_rtt - mrtt_us);
}

void tcp_bound_rto(tju_tcp_t* sock) {
    if(sock->rtt_timer.timeout > TCP_RTO_MAX) {
        sock->rtt_timer.timeout = TCP_RTO_MAX;
    }
}

void tcp_set_rto(tju_tcp_t* sock) {
    sock->rtt_timer.timeout = sock->rtt_timer.estimated_rtt = max(CLOCK_G, 4 * sock->rtt_timer.dev_rtt);
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