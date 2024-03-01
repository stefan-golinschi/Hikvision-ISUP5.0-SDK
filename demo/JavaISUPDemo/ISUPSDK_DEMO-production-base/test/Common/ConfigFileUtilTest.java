package Common;

import java.util.HashMap;
import java.util.Map;

/**
 * @author zhengxiaohui
 * @date 2023/8/15 19:09
 * @desc
 */
public class ConfigFileUtilTest {


    public static void testReadXml() {

        Map<String, Object> parameter = new HashMap<>();
        parameter.put("employeeNo", "989898");
        parameter.put("name", "测试账号98");
        parameter.put("enable", true);
        parameter.put("doorNo", 1);

        System.out.println(ConfigFileUtil.getReqBodyFromTemplate("conf/acs/AddUserInfoParam.json", parameter));
    }

    public static void main(String[] args) {
        testReadXml();
//        print();

//        System.out.println(System.getProperty("user.dir") + "\\video\\realplayData.mp4");
//        testPutServerParam();
    }

    public static void print() {
        String test ="{\n" +
                "\"PictureServerInformation\":{\n" +
                "\"pictureServerType\":\"cloudStorage\", \n" +
                "\"addressingFormatType\":\"ipaddress\",\n" +
                "\"hostName\":\"\",\n" +
                "\"ipv4Address\":\"" +  1111 + "\",\n" +
                "\"ipv6Address\":\"0\",\n" +
                "\"portNo\":" + 2222 + ",\n" +
                "\"cloudStorage\":{\n" +
                "\"cloudManageHttpPort\":6011,\n" +
                "\"cloudTransDataPort\":6011,\n" +
                "\"cloudCmdPort\":6011,\n" +
                "\"cloudHeartBeatPort\":6011,\n" +
                "\"cloudStorageHttpPort\":6011,\n" +
                "\"cloudUsername\":\"admin\",\n" +
                "\"cloudPassword\":\"12345\",\n" +
                "\"cloudPoolId\":1,\n" +
                "\"clouldProtocolVersion\":\"cloud2\",\n" +
                "\"clouldAccessKey\":\"khchhwtnmdqhwczjpmgmgykerdtwfywphazqwudkzvtvaserfmdnkpueuncalars\",\n" +
                "\"clouldSecretKey\":\"bqizmnkljfhlyncfafsmumqpcurnviiybsxnijztriepvgwxnqkjdcafzruuceud\"\n" +
                "}}}";
        System.out.println(test);
    }


    public static void testPutServerParam() {
        Map<String, Object> parameter = new HashMap<>();
        parameter.put("ipv4Address", "19.19.19.19");
        parameter.put("portNo", Short.parseShort("9099"));
        String input = ConfigFileUtil.getReqBodyFromTemplate("conf/SdkCms/PutPicServerParam.json", parameter);
        System.out.println(input);
    }
}