# 服务端

目前实现的功能有：登录，注册，找回密码，加好友，获取好友列表，私聊。

暂不支持头像信息和聊天记录暂存

## 编译

首先运行Server文件夹下的env.sh，安装环境

```shell
sh env.sh
```

在Server文件夹下，运行build.sh

```shell
sh build.sh
```

## 运行

- 我们自己用的（发布的时候删掉）

在Server文件夹下，运行run.sh（这个是调试用的）

```shell
sh run.sh
```

- 发布的

进入Server/bin文件夹，运行Server

```shell
./Server
```

## 数据表

```sql
create table User(
    uName varchar(32) primary key,
    pWord varchar(32) not null,
    phonenum varchar(11) unique not null,
    secureQue int not null,
    secureAns text not null,
    headfile text default ''
);

create table Friends(
    username varchar(32) not null,
    friend varchar(32) not null,
    primary key (username, friend),
    foreign key (username) references User(uName),
    foreign key (friend) references User(uName)
);

create table GroupInfo(
    groupid varchar(32) primary key,
    groupname varchar(32) not null
);

create table Groups(
    username varchar(32) not null,
    groupid varchar(32) not null,
    primary key (username, groupid),
    foreign key (username) references User(uName),
    foreign key (groupid) references GroupInfo(groupid)
);

select username from Group where groupid = '123123';

create trigger drop_user before delete
on User
begin
    delete from Friends where username = old.uName or friend = old.uName;
end;

create trigger drop_group before delete
on GroupInfo
begin
    delete from Groups where groupid = old.groupid;
end;

create trigger add_friends after insert on Friends
begin
    insert into Friends values(new.friend, new.username);
end;

```

## 指令测试：

```json
{
    "type": "regist",
    "info": {
        "username": "jack",
        "password": "123",
        "phonenum": "12345678900",
        "secureQue": 1,
        "secureAns":"fuck"
    }
}

{
    "type": "login",
    "info": {
        "username": "andy",
        "password": "123"
    }
}

{
    "type": "chat",
    "info": {
        "username": "andy",
        "userList": ["andy"],
        "info": "nh"
    }
}

{
    "type": "addfriends",
    "info": {
        "username": "andy",
        "friend_username": "jack"
    }
}

{
    "type": "askfriendsList",
    "info": {
        "username": "andy"
    }
}

{
    "type": "findpWord1",
    "info": {
        "username": "andy",
        "phonenum": "12345678900"
    }
}

{
    "type": "findpWord2",
    "info": {
        "username": "andy",
        "secureAns": "fuck"
    }
}

{
    "type": "findpWord3",
    "info": {
        "username": "andy",
        "newpWord": "123456"
    }
}

{
    "type": "cancelFindpWord",
    "info": {
        "username": "andy",
    }
}

```

## 文件指令

- 服务器收文件


```json
// from fileClient:
{
    "type": "sendFile",
    "info": {
        "username": "andy",
        "filename": "abc.jpg",
        "size": 1024
    }
}

// from fileClient:
asflasdfjlaksfjddljfk // a stream of bits.

// to fileClient: 
{
    "type": "sendState",
    "info":{
        "state": 1
    }
}
```

- C->S：

```json

// from fileClient
{
    "type": "reqFile",
    "info": {
        "fileFd": 1,
    }
}

//to client
{
    "type": "sendFile",
    "info": {
        // does file exist.
        "state": 1,
        "fileName": "../res/jack/head.jpg",
        "size": 4096,
    }
}

// to fileClient
asdfasdfadsfa // a stream of bits

// to fileClient
{
    "type": "sendState",
    "info": {
        "state": 1
    }
}
```

../res/users/jack/085850oxmmuxnhn2z2hdn8.jpeg