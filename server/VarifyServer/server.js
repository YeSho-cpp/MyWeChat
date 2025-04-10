// 导入必要的模块（假设这些已在文件顶部导入）
const grpc = require('@grpc/grpc-js')
const message_proto = require('./proto')
const const_module = require('./const')
const { v4: uuidv4 } = require('uuid');
const emailModule = require('./email');
const redis_module = require('./redis')

/**
 * 获取验证码的异步函数
 * @param {Object} call - gRPC call 对象，包含请求信息
 * @param {function} callback - gRPC 回调函数，用于发送响应
 */
async function GetVarifyCode(call, callback) {
  console.log("email is ", call.request.email);
  try {

    let query_res = await redis_module.GetRedis(const_module.code_prefix+call.request.email);
    console.log("query_res is ", query_res)
    let uniqueId = query_res;
    if(query_res == null){
      // 生成唯一的验证码
      uniqueId = uuidv4();
      if (uniqueId.length > 4) uniqueId = uniqueId.substring(0, 4); // 将验证码缩短

      let bres = await redis_module.SetRedisExpire(const_module.code_prefix+call.request.email, uniqueId,600) // 设置过期时间

      if(!bres){
        callback(null, { email:  call.request.email,
            error:const_module.Errors.RedisErr
        });
        return;
      }
    }

    console.log("uniqueId is ", uniqueId);

    // 构造邮件内容
    let text_str = '您的验证码为' + uniqueId + '请三分钟内完成注册';

    // 配置邮件选项
    let mailOptions = {
      from: 'm18736361595@163.com', // 发件人邮箱
      to: call.request.email,       // 收件人邮箱（从请求中获取）
      subject: '验证码',            // 邮件主题
      text: text_str,               // 邮件正文
    };

    // 发送邮件
    let send_res = await emailModule.SendMail(mailOptions); // 等待一个 Promise 解决（resolve）或拒绝（reject）。
    console.log("send res is ", send_res);

    // 发送响应
    if(!send_res){
      callback(null, { email:  call.request.email,
        error:const_module.Errors.RedisErr
      }); 
      return;
    }

    callback(null, { email:  call.request.email,
      error:const_module.Errors.Success
    }); 

  } catch (error) {
    // 处理错误
    console.log("catch error is ", error);
    callback(null, {
      email: call.request.email,
      error: const_module.Errors.Exception
    });
  }
}

/**
 * 主函数，用于设置和启动 gRPC 服务器
 */
function main() {
  // 创建 gRPC 服务器实例
  var server = new grpc.Server(); // 监听客户端发过来的信息，grpc-client调用过来的信息

  // 添加验证服务到服务器
  server.addService(message_proto.VarifyService.service, { GetVarifyCode: GetVarifyCode }); // { GetVarifyCode: GetVarifyCode } 是一个对象，将服务描述中定义的方法名映射到实际的实现函数。

  // 绑定服务器到指定地址和端口
  server.bindAsync('0.0.0.0:50051', grpc.ServerCredentials.createInsecure(), () => {
    // 启动服务器
    // server.start();
    console.log('grpc server started');
  });
}

// 调用主函数启动服务器
main();