<%@page import="com.loger.chart.line.LineChart12"%>
<%@ page import="com.loger.chart.line.LineChart1" %>
<%@ page language="java" contentType="text/html; charset=utf-8"
         pageEncoding="utf-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title>Insert title here</title>
</head>
<body>
<%
    String fileName= LineChart1.genLineChart(session);
    System.out.println(fileName);

%>
<img src="/img/<%=fileName %>" width="700" height="500" border="0"/>
</body>
</html>