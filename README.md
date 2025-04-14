# C++ 高性能聊天室服务器

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

基于Gyanis框架构建的现代实时通信系统，结合HTTP资源服务与WebSocket实时通信能力，实现高性能聊天室服务。单机支持万级并发连接，毫秒级消息延迟。

## 🌟 核心功能

- **即时通讯**：支持文本/表情/文件传输（<50MB）
- **多聊天室**：动态创建/加入/退出聊天室，房间人数无上限

## 📦 技术架构

### 系统组成
```
+-------------------------------+
|          Chat Room APP        |
+---------------+---------------+
| HTTP Server   | WebSocket     |
| (REST API)    | (实时通信)      |
+---------------+---------------+
|        Gyanis Framework       |
| (协程调度/异步IO/协议栈)         |
+-------------------------------+
|          System Layer         |
| (Linux epoll/线程池/TCP栈)     |
+-------------------------------+
```

### 关键技术栈
- **网络层**：Gyanis协程调度引擎（8 worker threads）
- **协议层**：HTTP/1.1 + WebSocket（基于Ragel的状态机解析）

## ⚙️ 依赖环境

### 必需组件
- Gyanis Framework






