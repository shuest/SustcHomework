package com.loger.chart.line;
import java.sql.*;
import java.util.List;
import java.awt.Color;
import java.awt.Font;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.sql.Connection;
import java.sql.SQLException;
import java.text.DecimalFormat;

import com.fantai.entity.SensorInfo;
import com.fantai.entity.UserInfo;
import com.fantai.util.DatabaseUtil;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartFrame;
import org.jfree.chart.ChartUtilities;
import org.jfree.chart.JFreeChart;
//import org.jfree.chart.StandardChartTheme;
import org.jfree.chart.axis.CategoryAxis;
import org.jfree.chart.axis.CategoryLabelPositions;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.labels.StandardCategoryItemLabelGenerator;
import org.jfree.chart.plot.CategoryPlot;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.renderer.category.LineAndShapeRenderer;
import org.jfree.chart.servlet.ServletUtilities;
import org.jfree.data.category.CategoryDataset;
import org.jfree.data.category.DefaultCategoryDataset;
import org.jfree.data.general.DatasetUtilities;
import org.jfree.ui.RectangleInsets;
import org.junit.Test;

import javax.servlet.http.HttpSession;

public class LineChart1 {


    //数据采集 （被调用）
    private static CategoryDataset createDataset() throws SQLException {
        DefaultCategoryDataset localDefaultCategoryDataset = new DefaultCategoryDataset();
        //从数据库查
        Connection conn = DatabaseUtil.getConnection();
        Statement stmt= conn.createStatement();
        String sql = "SELECT\n" +
                "  `se_si_id`,\n" +
                "  `sy_di_code`,\n" +
                "  `se_si_tem`,\n" +
                "  `se_si_hum`,\n" +
                "  `se_si_pressure`,\n" +
                "  `se_si_imei`,\n" +
                "  `se_si_sim`,\n" +
                "  `se_si_online`,\n" +
                "  `se_si_electric`,\n" +
                "  `se_si_time`,\n" +
                "  `se_si_up_time`\n" +
                "FROM\n" +
                "  `fantainb1`.`se_sensorinfo`\n" ;
        ResultSet rs = stmt.executeQuery(sql);
        int [] se_id = new int[1000];
        double [] se_tem = new double [1000];
        double [] se_hum = new double [1000];
        double [] se_pressure = new double[1000];
        int num = 0;
        while(rs.next()){
            // 通过字段检索
            int id  = rs.getInt("se_si_id");
            double se_si_tem = rs.getDouble("se_si_tem");
            double se_si_hum = rs.getDouble("se_si_hum");
            double se_si_pressure = rs.getDouble("se_si_pressure");

            // 保存数据
            se_id[num] = id;
            se_tem[num] = se_si_tem;
            se_hum[num] = se_si_hum;
            se_pressure[num] = se_si_pressure;
            num++;
        }


        //第一条折线数据 折线名Series 1
        for(int i = 1; i <= 30; i++) {
            Integer it = new Integer(i);
            String s = " " + it.toString() + " ";
            double data =  se_tem[num - 30 + i-1];
            localDefaultCategoryDataset.addValue(data, "温度", s);
        }
        for(int i = 1; i <= 30; i++) {
            Integer it = new Integer(i);
            String s = " " + it.toString() + " ";
            double data = se_hum[num - 30 + i-1];
            localDefaultCategoryDataset.addValue(data, "湿度", s);
        }
        for(int i = 1; i <= 30; i++) {
            Integer it = new Integer(i);
            String s = " " + it.toString() + " ";
            double data = se_pressure[num - 30 + i-1];
            localDefaultCategoryDataset.addValue(data, "大气压", s);
        }

        return localDefaultCategoryDataset;
    }
    /**
     * 生成折线图（被调用）
     * @param chartTitle 图的标题
     * @param x          横轴标题
     * @param y          纵轴标题
     * @param dataset    数据集
     * @return
     */
    private static JFreeChart createChart(String chartTitle, String x,
                                          String y, CategoryDataset dataset)
    {
        // 构建一个chart
        JFreeChart chart = ChartFactory.createLineChart(
                chartTitle,//图的标题
                x,  //x轴标题
                y,  //y轴标题
                dataset,//数据点
                PlotOrientation.VERTICAL,
                true,
                true,
                false);
        //字体清晰
       // chart.setTextAntiAlias(false);
        // 设置背景颜色
        chart.setBackgroundPaint(Color.WHITE);

        // 设置图标题的字体
        Font font = new Font("隶书", Font.BOLD, 25);
        chart.getTitle().setFont(font);

        // 设置面板字体
        Font labelFont = new Font("SansSerif", Font.TRUETYPE_FONT, 12);
        // 设置图示的字体
        chart.getLegend().setItemFont(labelFont);

        CategoryPlot categoryplot = (CategoryPlot) chart.getPlot();
        // x轴 // 分类轴网格是否可见
        categoryplot.setDomainGridlinesVisible(true);
        // y轴 //数据轴网格是否可见
        categoryplot.setRangeGridlinesVisible(true);
        categoryplot.setRangeGridlinePaint(Color.WHITE);// 虚线色彩
        categoryplot.setDomainGridlinePaint(Color.WHITE);// 虚线色彩
        categoryplot.setBackgroundPaint(Color.lightGray);// 折线图的背景颜色

        // 设置轴和面板之间的距离
        // categoryplot.setAxisOffset(new RectangleInsets(5D, 5D, 5D, 5D));

        // 横轴 x
        CategoryAxis domainAxis = categoryplot.getDomainAxis();
        domainAxis.setLabelFont(labelFont);// 轴标题
        domainAxis.setTickLabelFont(labelFont);// 轴数值
        // domainAxis.setLabelPaint(Color.BLUE);//轴标题的颜色
        // domainAxis.setTickLabelPaint(Color.BLUE);//轴数值的颜色

        // 横轴 lable 的位置 横轴上的 Lable 45度倾斜 DOWN_45
        domainAxis.setCategoryLabelPositions(CategoryLabelPositions.STANDARD);

        // 设置距离图片左端距离
        domainAxis.setLowerMargin(0.0);
        // 设置距离图片右端距离
        domainAxis.setUpperMargin(0.0);

        // 纵轴 y
        NumberAxis numberaxis = (NumberAxis) categoryplot.getRangeAxis();
        numberaxis.setLabelFont(labelFont);
        numberaxis.setTickLabelFont(labelFont);
        numberaxis.setStandardTickUnits(NumberAxis.createIntegerTickUnits());
        numberaxis.setAutoRangeIncludesZero(true);

        // 获得renderer 注意这里是下嗍造型到lineandshaperenderer！！
        LineAndShapeRenderer lineandshaperenderer = (LineAndShapeRenderer) categoryplot
                .getRenderer();
        lineandshaperenderer.setBaseShapesVisible(true); // series 点（即数据点）可见
        lineandshaperenderer.setBaseLinesVisible(true); // series 点（即数据点）间有连线可见

        // 显示折点数据
        lineandshaperenderer
                .setBaseItemLabelGenerator(new StandardCategoryItemLabelGenerator());
        lineandshaperenderer.setBaseItemLabelsVisible(true);

        return chart;
    }

    //获取JFreeChart
    public static JFreeChart createDemoPanel() throws SQLException {
        CategoryDataset database = createDataset();
        JFreeChart localJFreeChart = createChart("数据折线图展示","次数","最近30次观测数据制图",database);

        return localJFreeChart;
    }
    public static String genLineChart(HttpSession session)throws Exception {
        JFreeChart chart = createDemoPanel();

        String fileName= ServletUtilities.saveChartAsPNG(chart, 700, 500, session);
        //ServletUtilities.setTempFilePrefix("public-jfreechart-");

        FileInputStream input=new FileInputStream("D:\\Program Files (x86)\\apache-tomcat-7.0.85-windows-x64\\apache-tomcat-7.0.85\\temp\\"+fileName);
        FileOutputStream output=new FileOutputStream("D:\\Program Files (x86)\\apache-tomcat-7.0.85-windows-x64\\apache-tomcat-7.0.85\\webapps\\ROOT\\img\\"+fileName);
        int in=input.read();
        while(in!=-1){
            output.write(in);
            in=input.read();
        }
        input.close();
        output.close();

        return fileName;

    }

}
