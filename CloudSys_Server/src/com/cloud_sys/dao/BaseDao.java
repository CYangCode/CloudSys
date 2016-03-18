package com.cloud_sys.dao;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.Set;

import com.mysql.jdbc.ResultSetMetaData;

public class BaseDao {

	//private String url = "jdbc:mysql://49.140.58.25:3306/";
	private String url = "jdbc:mysql://127.0.0.1:3306/";
	private String dbs = "cloud_sys";
	private String user = "root";
	private String pwd = "1234";

	private Connection connection;
	private PreparedStatement preparedStatement;
	private Statement statement;

	public BaseDao() {
		try {
			connection = DriverManager.getConnection(url + dbs, user, pwd);
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}

	static {
		try {
			Class.forName("com.mysql.jdbc.Driver");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void close() {
		if (connection != null) {
			try {
				connection.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
		if (preparedStatement != null) {
			try {
				preparedStatement.close();
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		if (statement != null) {
			try {
				statement.close();
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	public int executeUpdate(String sql, String[] args) {
		try {
			preparedStatement = connection.prepareStatement(sql);
			for (int i = 0; i < args.length; i++) {
				preparedStatement.setObject(i + 1, args[i]);
			}
			return preparedStatement.executeUpdate();
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return -1;
	}

	public int executeUpdate(String sql) {
		try {
			statement = connection.createStatement();
			return statement.executeUpdate(sql);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return -1;
	}

	public ArrayList<HashMap<String, String>> executeQuery(String sql,
			String[] args) {
		ArrayList<HashMap<String, String>> list = new ArrayList<HashMap<String, String>>();
		try {
			preparedStatement = connection.prepareStatement(sql);
			for (int i = 0; i < args.length; i++) {
				preparedStatement.setObject(i + 1, args[i]);
			}
			ResultSet resultSet = preparedStatement.executeQuery();
			ResultSetMetaData rsmd = (ResultSetMetaData) resultSet
					.getMetaData();
			int columnCount = rsmd.getColumnCount();
			HashMap<String, String> resultMap = null;
			while (resultSet.next()) {
				for (int i = 1; i <= columnCount; i++) {
					resultMap = new HashMap<String, String>();
					resultMap
							.put(rsmd.getColumnName(i), resultSet.getString(i));
				}
				list.add(resultMap);
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return null;
	}

	public ArrayList<HashMap<String, String>> executeQuery(String sql) {
		ArrayList<HashMap<String, String>> list = new ArrayList<HashMap<String, String>>();
		try {
			statement = connection.createStatement();
			ResultSet resultSet =  statement.executeQuery(sql);
			ResultSetMetaData rsmd = (ResultSetMetaData) resultSet
					.getMetaData();
			int columnCount = rsmd.getColumnCount();
			HashMap<String, String> resultMap = null;
			while (resultSet.next()) {
				resultMap = new HashMap<String, String>();
				for (int i = 1; i <= columnCount; i++) {
					resultMap
							.put(rsmd.getColumnName(i), resultSet.getString(i));
				}
				list.add(resultMap);
			}
			return list;
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return null;
	}

	public static void main(String[] args) {
		String sql = "select * from `user` where user_name='cy'";
		BaseDao tool = new BaseDao();
		ArrayList<HashMap<String, String>> list = tool.executeQuery(sql);
		HashMap<String, String> result = list.get(0);
		Set<Entry<String, String>> set = result.entrySet();
		for (Entry<String, String> entry: set) { 
System.out.println("Key: " + entry.getKey() + " Value: " + entry.getValue());
		}
		tool.close();
	}
}
