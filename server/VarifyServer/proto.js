const path = require('path')
const grpc = require('@grpc/grpc-js')
const protoLoader = require('@grpc/proto-loader')

// 使用 path.join 来构建 proto 文件的路径，这样可以确保跨平台兼容性
const PROTO_PATH = path.join(__dirname, 'message.proto')

// 加载 proto 文件
const packageDefinition = protoLoader.loadSync(PROTO_PATH, {
    keepCase: true, // 保持字段名称的原始大小写，不进行任何转换。
    longs: String, // 指定如何表示长整型
    enums: String,
    defaults: true,
    oneofs: true
})

// 将加载的定义转换为 gRPC 对象
const protoDescriptor = grpc.loadPackageDefinition(packageDefinition)

// 获取 message 包（假设你的 proto 文件定义了一个名为 message 的包）
const message_proto = protoDescriptor.message

// 导出 message_proto 对象，使其可以在其他文件中使用
module.exports = message_proto