create table if not exists Account(
PrjName varchar(256) PRIMARY KEY,
PrjDesc varchar(256),
UserType varchar(256) Default 'Admin' NOT NULL,
UserName varchar(256) Default 'Admin' NOT NULL,
UserPassword varchar(256) Default 'gopds' NOT NULL,
PrjCode varchar(256),
ClientName varchar(256),
Folder varchar(256) NOT NULL,
Host varchar(256),
ID varchar(256),
Password varchar(256)
)
