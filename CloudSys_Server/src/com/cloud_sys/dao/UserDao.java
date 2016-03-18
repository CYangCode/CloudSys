package com.cloud_sys.dao;

import java.util.HashMap;

import com.cloud_sys.entity.*;

public class UserDao extends BaseDao {
	/**
	 * 通过实体类添加到数据库
	 * @param user 包含用户信息的实体对象
	 * @return 1代表执行正确
	 */
	public int addUser(User user) {
		String sql = "insert into user (`user_name`, `user_pwd`, `user_phone`, `user_mail`) values (?,?,?,?)";
		String[] param = { user.getName(), user.getPassword(),
				user.getPhone(), user.getEmail()};
		return executeUpdate(sql, param);
	}
	
	/**
	 * 通过用户名(主键)查找用户
	 * @param userName 用户名
	 * @return 对应该用户名的用户实体对象
	 */
	public User findUserByUserName(String userName) {
		String sql = "select * from user where user_name='" + userName + "'";
		HashMap<String, String> result = executeQuery(sql).get(0);
		User user = new User();
		user.setName(result.get("user_name"));
		user.setPassword(result.get("user_pwd"));
		user.setPhone(result.get("user_phone"));
		user.setEmail(result.get("user_mail"));
		return user;
	}
	
	/**
	 * 通过用户名获得用户密码
	 * @param userName 用户名
	 * @return 用户密码
	 */
	public String findUserPwdByUserName(String userName){
		String sql = "select user_pwd from user where user_name='" + userName + "'";
		HashMap<String, String> result = executeQuery(sql).get(0);
		return result.get("user_pwd");
	}
	
	public static void main(String[] args) {
		//测试findUserPwdByUserName的方法
//		UserDao userDao = new UserDao();
//		String userPwd = userDao.findUserPwdByUserName("cy");
//		System.err.println(userPwd);
//		
//		//测试findUserByUserName方法
//		User user = new UserDao().findUserByUserName("cy");
//		System.out.println(user);
		
		//测试addUser方法
		User user = new User();
		user.setName("yjx2");
		user.setPassword("123");

		user.setPhone("731287812");
		user.setEmail("hahah@qq.com");
		UserDao userDao = new UserDao();
		userDao.addUser(user);
	}
	
}
