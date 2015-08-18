package com.cloud_sys.dao;

import java.util.HashMap;

import com.cloud_sys.entity.*;

public class UserDao extends BaseDao {
	/**
	 * ͨ��ʵ������ӵ����ݿ�
	 * @param user �����û���Ϣ��ʵ�����
	 * @return 1����ִ����ȷ
	 */
	public int addUser(User user) {
		String sql = "insert into user values (?,?,?,?,?,?)";
		String[] param = { user.getName(), user.getPassword(),
				user.getIdCard(), user.getGender(), user.getAddress(),
				user.getPhone() };
		return executeUpdate(sql, param);
	}
	
	/**
	 * ͨ���û���(����)�����û�
	 * @param userName �û���
	 * @return ��Ӧ���û������û�ʵ�����
	 */
	public User findUserByUserName(String userName) {
		String sql = "select * from user where user_name='" + userName + "'";
		HashMap<String, String> result = executeQuery(sql).get(0);
		User user = new User();
		user.setName(result.get("user_name"));
		user.setPassword(result.get("user_pwd"));
		user.setIdCard(result.get("user_idcard"));
		user.setGender(result.get("user_gender"));
		user.setAddress(result.get("user_address"));
		user.setPhone(result.get("user_phone"));
		return user;
	}
	
	/**
	 * ͨ���û�������û�����
	 * @param userName �û���
	 * @return �û�����
	 */
	public String findUserPwdByUserName(String userName){
		String sql = "select user_pwd from user where user_name='" + userName + "'";
		HashMap<String, String> result = executeQuery(sql).get(0);
		return result.get("user_pwd");
	}
	
	public static void main(String[] args) {
		//����findUserPwdByUserName�ķ���
//		UserDao userDao = new UserDao();
//		String userPwd = userDao.findUserPwdByUserName("cy");
//		System.err.println(userPwd);
//		
		//����findUserByUserName����
//		User user = new UserDao().findUserByUserName("cy");
//		System.out.println(user);
		
		//����addUser����
//		User user = new User();
//		user.setName("chf");
//		user.setPassword("123");
//		user.setAddress("����ʡ������");
//		user.setGender("M");
//		user.setIdCard("72347947834203");
//		user.setPhone("18844546470");
//		UserDao userDao = new UserDao();
//		userDao.addUser(user);
	}
	
}
