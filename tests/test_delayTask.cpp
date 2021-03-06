﻿/*
 * MIT License
 *
 * Copyright (c) 2016 xiongziliang <771730766@qq.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <signal.h>
#include <iostream>
#include "Util/util.h"
#include "Util/logger.h"
#include "Util/TimeTicker.h"
#include "Poller/EventPoller.h"
#include "Thread/AsyncTaskThread.h"

using namespace std;
using namespace toolkit;

/**
 * cpu负载均衡测试
 * @return
 */
int main() {
	//设置日志
	Logger::Instance().add(std::make_shared<ConsoleChannel>());
    Logger::Instance().setWriter(std::make_shared<AsyncLogWriter>());

    Ticker ticker0;
	int nextDelay0 = 5;
	auto tag0 = EventPollerPool::Instance().getPoller()->doTaskDelay(nextDelay0,[&](){
		TraceL << "task 0,预期休眠时间 :"  << nextDelay0 << " 实际休眠时间" << ticker0.elapsedTime();
		ticker0.resetTime();
		return nextDelay0;
	});

	Ticker ticker1;
	int nextDelay1 = 5;
    auto tag1 = EventPollerPool::Instance().getPoller()->doTaskDelay(nextDelay1,[&](){
		DebugL << "task 1,预期休眠时间 :" << nextDelay1 << " 实际休眠时间" << ticker1.elapsedTime();
		ticker1.resetTime();
		nextDelay1 += 1;
		return nextDelay1;
	});

    Ticker ticker2;
    int nextDelay2 = 3000;
    auto tag2 = EventPollerPool::Instance().getPoller()->doTaskDelay(nextDelay2,[&](){
        InfoL << "task 2,预期休眠时间 :" << nextDelay2 << " 实际休眠时间" << ticker2.elapsedTime();
        return 0;
    });

	sleep(1);
    tag0->cancel();
    tag1->cancel();
    DebugL << "取消task 0、1";

	static semaphore sem;
	signal(SIGINT, [](int) { sem.post(); });// 设置退出信号
	sem.wait();

	return 0;
}
