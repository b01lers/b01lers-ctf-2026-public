
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

unsigned int w_line = 0;
int write_line(char *str, int pos);
void revert_file();
void clear_before(unsigned int line);
void clear_before_n(unsigned int line, int n);
void leave(int code);
void decode_frame(int input_idx);
const char flag[] = "\x04\x97\xbe\xc2\xfa\x5e\xf5\x59\x5d\xc1\x94\x00\x2e\xb8\x74\xe7\x41\x1c\xc9\x01\x98\x2f\xb3\xc7\xfa\x84\x40\xc5\xd6\x46\x78\x13";
const int flag_len = 32;

void do_op(int argc, char** argv){

  if (argc < 3) {
    revert_file();
    write_line("int () {", 1);
    write_line("./chall $LINENO 999999", 2);
    write_line("exit", 3);
    write_line("}", 4);
    write_line("trap \"int\" INT", 5);
    write_line("./chall $LINENO 9823", 6);
    return;
  }


  char output[200];
  int stack[10] = {0};
  int tmp;
  int line_no = atoi(argv[1]) - 1;
  int id = atoi(argv[2]);
  for (int i = 0; i < argc - 3; i++) {
    stack[i] = atoi(argv[i + 3]);
  }

  if (line_no > 250 && line_no % 3 == 0) {
    double frame = (line_no - 250) / 5200.0 * 2190.0;
    if (frame > 0 && frame < 2190) {
      decode_frame((int) frame);
    }
  }


  void *jumptable[] = {&&inst_9707, &&inst_9987, &&inst_6976, &&inst_4944, &&inst_9681, &&inst_9690, &&inst_3559, &&inst_397, &&inst_6470, &&inst_195, &&inst_7241, &&inst_223, &&inst_3453, &&inst_289, &&inst_9136, &&inst_8639, &&inst_5314, &&inst_5082, &&inst_2828, &&inst_5159, &&inst_5631, &&inst_6041, &&inst_7760, &&inst_3614, &&inst_9038, &&inst_5247, &&inst_8245, &&inst_1272, &&inst_8631, &&inst_1391, &&inst_2236, &&inst_8051, &&inst_8026, &&inst_5707, &&inst_8369, &&inst_2014, &&inst_9615, &&inst_6907, &&inst_9478, &&inst_5858, &&inst_4880, &&inst_5098, &&inst_7142, &&inst_6429, &&inst_981, &&inst_1314, &&inst_257, &&inst_8351, &&inst_4390, &&inst_5042, &&inst_1030, &&inst_75, &&inst_5997, &&inst_4950, &&inst_6671, &&inst_7528, &&inst_4001, &&inst_8107, &&inst_1266, &&inst_6786, &&inst_9861, &&inst_7202, &&inst_2133, &&inst_9160, &&inst_5328, &&inst_5609, &&inst_5207, &&inst_1508, &&inst_3151, &&inst_4975, &&inst_4320, &&inst_1443, &&inst_9032, &&inst_2008, &&inst_8974, &&inst_929, &&inst_7053, &&inst_2253, &&inst_1084, &&inst_3554, &&inst_4805, &&inst_2836, &&inst_8222, &&inst_4099, &&inst_9745, &&inst_4376, &&inst_2461, &&inst_3997, &&inst_8951, &&inst_1831, &&inst_5811, &&inst_9701, &&inst_6780, &&inst_7226, &&inst_7064, &&inst_9301, &&inst_6750, &&inst_5410, &&inst_1823, &&inst_5784, &&inst_4853, &&inst_1200, &&inst_9577, &&inst_2749, &&inst_2925, &&inst_6929, &&inst_8648, &&inst_860, &&inst_5510, &&inst_2506, &&inst_9569, &&inst_6448, &&inst_7987, &&inst_4008, &&inst_2220, &&inst_3521, &&inst_9958, &&inst_6931, &&inst_1397, &&inst_5428, &&inst_8636, &&inst_9799, &&inst_1526, &&inst_8225, &&inst_9302, &&inst_3451, &&inst_8148, &&inst_4524, &&inst_6322, &&inst_9130, &&inst_2958, &&inst_7612, &&inst_4764, &&inst_8197, &&inst_3874, &&inst_5312, &&inst_2167, &&inst_5369, &&inst_3046, &&inst_4299, &&inst_1561, &&inst_4591, &&inst_2939, &&inst_4612, &&inst_2846, &&inst_5539, &&inst_5384, &&inst_6264, &&inst_1603, &&inst_8716, &&inst_9242, &&inst_5588, &&inst_9731, &&inst_9023, &&inst_2212, &&inst_3091, &&inst_3975, &&inst_6177, &&inst_3193, &&inst_1993, &&inst_6601, &&inst_3200, &&inst_893, &&inst_5180, &&inst_2316, &&inst_6732, &&inst_1482, &&inst_8753, &&inst_6312, &&inst_503, &&inst_4744, &&inst_1206, &&inst_3042, &&inst_6219, &&inst_4373, &&inst_6589, &&inst_91, &&inst_866, &&inst_5653, &&inst_6823, &&inst_9571, &&inst_2501, &&inst_2344, &&inst_586, &&inst_5395, &&inst_5039, &&inst_5270, &&inst_3203, &&inst_6719, &&inst_4457, &&inst_5211, &&inst_385, &&inst_4074, &&inst_4190, &&inst_9305, &&inst_4852, &&inst_7828, &&inst_6970, &&inst_1972, &&inst_4075, &&inst_8620, &&inst_4613, &&inst_3160, &&inst_8494, &&inst_7843, &&inst_1628, &&inst_7155, &&inst_1181, &&inst_2339, &&inst_6793, &&inst_9734, &&inst_446, &&inst_623, &&inst_9682, &&inst_5904, &&inst_2027, &&inst_9686, &&inst_6306, &&inst_9448, &&inst_9826, &&inst_6785, &&inst_9128, &&inst_5339, &&inst_5786, &&inst_8885, &&inst_1372, &&inst_8400, &&inst_6535, &&inst_8233, &&inst_9074, &&inst_8055, &&inst_7562, &&inst_8997, &&inst_2296, &&inst_7220, &&inst_5366, &&inst_2966, &&inst_8778, &&inst_6622, &&inst_474, &&inst_1913, &&inst_7106, &&inst_2475, &&inst_1943, &&inst_4038, &&inst_8959, &&inst_6903, &&inst_8672, &&inst_596, &&inst_9622, &&inst_7663, &&inst_5290, &&inst_885, &&inst_761, &&inst_1247, &&inst_3987, &&inst_979, &&inst_7058, &&inst_6410, &&inst_1329, &&inst_1421, &&inst_1782, &&inst_8505, &&inst_6761, &&inst_3407, &&inst_2716, &&inst_609, &&inst_5417, &&inst_5881, &&inst_7588, &&inst_4628, &&inst_5271, &&inst_1241, &&inst_5960, &&inst_9487, &&inst_63, &&inst_6519, &&inst_6357, &&inst_8495, &&inst_7288, &&inst_2203, &&inst_4782, &&inst_2273, &&inst_1025, &&inst_1664, &&inst_7656, &&inst_5561, &&inst_2544, &&inst_9717, &&inst_5241, &&inst_6426, &&inst_1470, &&inst_7601, &&inst_2546, &&inst_634, &&inst_9115, &&inst_6285, &&inst_1949, &&inst_5516, &&inst_552, &&inst_3111, &&inst_1209, &&inst_9992, &&inst_995, &&inst_6404, &&inst_1413, &&inst_7699, &&inst_5456, &&inst_8508, &&inst_3353, &&inst_1656, &&inst_4443, &&inst_4769, &&inst_574, &&inst_6121, &&inst_3255, &&inst_5948, &&inst_4066, &&inst_9525, &&inst_3711, &&inst_1825, &&inst_6585, &&inst_4141, &&inst_7119, &&inst_8024, &&inst_6380, &&inst_5910, &&inst_1522, &&inst_3114, &&inst_6092, &&inst_1567, &&inst_8172, &&inst_32, &&inst_8918, &&inst_8234, &&inst_1110, &&inst_6765, &&inst_7594, &&inst_2422, &&inst_7573, &&inst_6816, &&inst_5070, &&inst_8214, &&inst_5065, &&inst_699, &&inst_4092, &&inst_8860, &&inst_4432, &&inst_1533, &&inst_8797, &&inst_3921, &&inst_1316, &&inst_171, &&inst_9800, &&inst_6810, &&inst_8314, &&inst_9952, &&inst_1354, &&inst_830, &&inst_2690, &&inst_3945, &&inst_9109, &&inst_9072, &&inst_2644, &&inst_7113, &&inst_8122, &&inst_9184, &&inst_5760, &&inst_1602, &&inst_3678, &&inst_8663, &&inst_4227, &&inst_243, &&inst_2858, &&inst_4995, &&inst_2169, &&inst_4823, &&inst_5237, &&inst_2932, &&inst_42, &&inst_184, &&inst_5485, &&inst_4098, &&inst_4766, &&inst_8257, &&inst_997, &&inst_68, &&inst_3899, &&inst_1671, &&inst_1556, &&inst_8186, &&inst_2065, &&inst_6399, &&inst_7387, &&inst_9463, &&inst_3932, &&inst_2951, &&inst_402, &&inst_1766, &&inst_9623, &&inst_5927, &&inst_3383, &&inst_8134, &&inst_9126, &&inst_6055, &&inst_1657, &&inst_6234, &&inst_8012, &&inst_8045, &&inst_5235, &&inst_7990, &&inst_7709, &&inst_4029, &&inst_432, &&inst_8700, &&inst_4281, &&inst_2592, &&inst_587, &&inst_3406, &&inst_9442, &&inst_5321, &&inst_5795, &&inst_4987, &&inst_1048, &&inst_1625, &&inst_7057, &&inst_7577, &&inst_9355, &&inst_8909, &&inst_1469, &&inst_3400, &&inst_8286, &&inst_2037, &&inst_2330, &&inst_9048, &&inst_8174, &&inst_7180, &&inst_7045, &&inst_2902, &&inst_4312, &&inst_120, &&inst_8414, &&inst_3581, &&inst_4495, &&inst_6624, &&inst_9779, &&inst_6629, &&inst_4733, &&inst_2490, &&inst_517, &&inst_7260, &&inst_4447, &&inst_1536, &&inst_9754, &&inst_1364, &&inst_4707, &&inst_9832, &&inst_8593, &&inst_8118, &&inst_5596, &&inst_9394, &&inst_9161, &&inst_125, &&inst_6330, &&inst_4936, &&inst_1679, &&inst_4384, &&inst_3984, &&inst_2447, &&inst_3194, &&inst_5616, &&inst_3381, &&inst_1163, &&inst_4300, &&inst_7309, &&inst_3231, &&inst_2, &&inst_7115, &&inst_6294, &&inst_619, &&inst_5430, &&inst_1207, &&inst_8221, &&inst_8379, &&inst_7484, &&inst_1359, &&inst_5404, &&inst_5851, &&inst_4550, &&inst_540, &&inst_5696, &&inst_1767, &&inst_757, &&inst_4679, &&inst_5117, &&inst_1325, &&inst_5717, &&inst_2620, &&inst_4232, &&inst_8612, &&inst_2070, &&inst_1256, &&inst_4693, &&inst_6506, &&inst_4031, &&inst_5458, &&inst_9138, &&inst_2057, &&inst_1435, &&inst_6955, &&inst_202, &&inst_5971, &&inst_3853, &&inst_2216, &&inst_7717, &&inst_6151, &&inst_2440, &&inst_6212, &&inst_478, &&inst_7811, &&inst_6592, &&inst_793, &&inst_147, &&inst_433, &&inst_3699, &&inst_5823, &&inst_9087, &&inst_4356, &&inst_6996, &&inst_1921, &&inst_3509, &&inst_5181, &&inst_6341, &&inst_5612, &&inst_6252, &&inst_216, &&inst_367, &&inst_7225, &&inst_7770, &&inst_3629, &&inst_6037, &&inst_1044, &&inst_8572, &&inst_7615, &&inst_6289, &&inst_6621, &&inst_4700, &&inst_6363, &&inst_5544, &&inst_2987, &&inst_4935, &&inst_3864, &&inst_7253, &&inst_5228, &&inst_2438, &&inst_4835, &&inst_7347, &&inst_9483, &&inst_1050, &&inst_8250, &&inst_9206, &&inst_2686, &&inst_6894, &&inst_5992, &&inst_1402, &&inst_2180, &&inst_8481, &&inst_9935, &&inst_4360, &&inst_2890, &&inst_2527, &&inst_745, &&inst_2935, &&inst_6221, &&inst_5387, &&inst_2277, &&inst_1778, &&inst_1337, &&inst_3883, &&inst_1689, &&inst_6284, &&inst_5964, &&inst_7322, &&inst_5266, &&inst_9658, &&inst_2351, &&inst_4221, &&inst_932, &&inst_436, &&inst_8790, &&inst_3721, &&inst_8985, &&inst_1963, &&inst_8499, &&inst_4627, &&inst_4686, &&inst_4800, &&inst_7565, &&inst_616, &&inst_245, &&inst_6891, &&inst_1052, &&inst_299, &&inst_9599, &&inst_9825, &&inst_3805, &&inst_3075, &&inst_8297, &&inst_7834, &&inst_4418, &&inst_1612, &&inst_982, &&inst_3518, &&inst_2354, &&inst_4181, &&inst_6981, &&inst_5289, &&inst_6598, &&inst_9782, &&inst_4565, &&inst_1695, &&inst_5526, &&inst_117, &&inst_544, &&inst_4370, &&inst_295, &&inst_9508, &&inst_1471, &&inst_2491, &&inst_1673, &&inst_1278, &&inst_4126, &&inst_2268, &&inst_2550, &&inst_871, &&inst_9173, &&inst_4982, &&inst_4234, &&inst_988, &&inst_6228, &&inst_5686, &&inst_149, &&inst_2075, &&inst_3242, &&inst_5918, &&inst_6420, &&inst_8128, &&inst_1730, &&inst_3477, &&inst_4732, &&inst_9291, &&inst_6849, &&inst_1324, &&inst_7172, &&inst_6756, &&inst_4260, &&inst_5016, &&inst_2091, &&inst_972, &&inst_900, &&inst_7511, &&inst_7632, &&inst_7737, &&inst_6434, &&inst_7065, &&inst_93, &&inst_795, &&inst_6325, &&inst_5801, &&inst_2563, &&inst_3437, &&inst_392, &&inst_2788, &&inst_3503, &&inst_8500, &&inst_3068, &&inst_7637, &&inst_6247, &&inst_4060, &&inst_5500, &&inst_2243, &&inst_8169, &&inst_5486, &&inst_3549, &&inst_6353, &&inst_8509, &&inst_4117, &&inst_5530, &&inst_9899, &&inst_3541, &&inst_4105, &&inst_4076, &&inst_2214, &&inst_4054, &&inst_5511, &&inst_7848, &&inst_3879, &&inst_6296, &&inst_8877, &&inst_4851, &&inst_3040, &&inst_8484, &&inst_3079, &&inst_9956, &&inst_1395, &&inst_4070, &&inst_152, &&inst_281, &&inst_8063, &&inst_357, &&inst_8601, &&inst_4866, &&inst_6936, &&inst_7686, &&inst_2923, &&inst_4945, &&inst_1924, &&inst_1497, &&inst_4361, &&inst_3096, &&inst_9202, &&inst_2157, &&inst_4846, &&inst_4622, &&inst_571, &&inst_258, &&inst_1941, &&inst_913, &&inst_3338, &&inst_9127, &&inst_3254, &&inst_5217, &&inst_3819, &&inst_1055, &&inst_2903, &&inst_7841, &&inst_9163, &&inst_9531, &&inst_2096, &&inst_5466, &&inst_6550, &&inst_3556, &&inst_9910, &&inst_1502, &&inst_8211, &&inst_1849, &&inst_5875, &&inst_4765, &&inst_5619, &&inst_4969, &&inst_1630, &&inst_3419, &&inst_2149, &&inst_1768, &&inst_950, &&inst_4770, &&inst_139, &&inst_5743, &&inst_691, &&inst_8108, &&inst_8196, &&inst_2874, &&inst_4364, &&inst_2730, &&inst_2824, &&inst_3717, &&inst_3616, &&inst_1151, &&inst_1600, &&inst_6883, &&inst_2683, &&inst_846, &&inst_7862, &&inst_3827, &&inst_6790, &&inst_9359, &&inst_9155, &&inst_8906, &&inst_9654, &&inst_4625, &&inst_6494, &&inst_9895, &&inst_5226, &&inst_7638, &&inst_6886, &&inst_864, &&inst_6082, &&inst_2373, &&inst_7271, &&inst_7206, &&inst_2068, &&inst_1394, &&inst_9695, &&inst_8943, &&inst_3783, &&inst_7863, &&inst_5778, &&inst_752, &&inst_3735, &&inst_4718, &&inst_8927, &&inst_1937, &&inst_9930, &&inst_2130, &&inst_7644, &&inst_9181, &&inst_1255, &&inst_3011, &&inst_9260, &&inst_7338, &&inst_2224, &&inst_9915, &&inst_5079, &&inst_8645, &&inst_8265, &&inst_3112, &&inst_4849, &&inst_5255, &&inst_2685, &&inst_7624, &&inst_9824, &&inst_7350, &&inst_8490, &&inst_2992, &&inst_7086, &&inst_9468, &&inst_3302, &&inst_945, &&inst_9587, &&inst_3395, &&inst_9004, &&inst_5739, &&inst_9860, &&inst_494, &&inst_7414, &&inst_2110, &&inst_8057, &&inst_7868, &&inst_5136, &&inst_6851, &&inst_9971, &&inst_6755, &&inst_1032, &&inst_441, &&inst_4508, &&inst_6697, &&inst_5990, &&inst_6701, &&inst_9398, &&inst_2225, &&inst_2888, &&inst_2878, &&inst_2876, &&inst_2496, &&inst_8483, &&inst_2116, &&inst_8009, &&inst_8850, &&inst_6781, &&inst_1280, &&inst_7580, &&inst_3201, &&inst_1752, &&inst_3448, &&inst_8469, &&inst_8638, &&inst_2412, &&inst_6662, &&inst_1714, &&inst_4339, &&inst_2599, &&inst_7996, &&inst_7762, &&inst_3120, &&inst_5915, &&inst_706, &&inst_4706, &&inst_4245, &&inst_4218, &&inst_1798, &&inst_424, &&inst_2134, &&inst_6848, &&inst_234, &&inst_9216, &&inst_3965, &&inst_8338, &&inst_4502, &&inst_8947, &&inst_277, &&inst_7973, &&inst_8939, &&inst_1073, &&inst_1725, &&inst_172, &&inst_7139, &&inst_1381, &&inst_7988, &&inst_5680, &&inst_1479, &&inst_9007, &&inst_7235, &&inst_9994, &&inst_4597, &&inst_5966, &&inst_1036, &&inst_8258, &&inst_8816, &&inst_5867, &&inst_9735, &&inst_3077, &&inst_7344, &&inst_3292, &&inst_522, &&inst_6834, &&inst_5665, &&inst_3370, &&inst_9808, &&inst_2262, &&inst_808, &&inst_7147, &&inst_8741, &&inst_4254, &&inst_5776, &&inst_5066, &&inst_8596, &&inst_239, &&inst_949, &&inst_6440, &&inst_4453, &&inst_9016, &&inst_696, &&inst_3423, &&inst_9591, &&inst_5493, &&inst_4813, &&inst_6943, &&inst_1161, &&inst_9933, &&inst_1418, &&inst_5441, &&inst_5202, &&inst_233, &&inst_267, &&inst_9510, &&inst_2539, &&inst_4653, &&inst_3090, &&inst_8133, &&inst_450, &&inst_9590, &&inst_2609, &&inst_9343, &&inst_5498, &&inst_2195, &&inst_5906, &&inst_7937, &&inst_4535, &&inst_8719, &&inst_3936, &&inst_5542, &&inst_8116, &&inst_5794, &&inst_7123, &&inst_5804, &&inst_9140, &&inst_5118, &&inst_4311, &&inst_8095, &&inst_2267, &&inst_969, &&inst_6445, &&inst_7718, &&inst_4746, &&inst_2505, &&inst_8656, &&inst_8610, &&inst_6246, &&inst_5017, &&inst_8894, &&inst_8843, &&inst_1926, &&inst_9718, &&inst_764, &&inst_6300, &&inst_5499, &&inst_7004, &&inst_2964, &&inst_3412, &&inst_901, &&inst_7867, &&inst_2759, &&inst_9998, &&inst_8551, &&inst_2334, &&inst_9928, &&inst_9802, &&inst_2156, &&inst_382, &&inst_393, &&inst_5043, &&inst_7689, &&inst_2390, &&inst_4030, &&inst_1813, &&inst_1579, &&inst_9834, &&inst_2777, &&inst_3409, &&inst_9806, &&inst_8920, &&inst_3748, &&inst_2985, &&inst_8831, &&inst_7754, &&inst_3673, &&inst_4736, &&inst_3771, &&inst_2561, &&inst_4521, &&inst_1631, &&inst_8982, &&inst_1312, &&inst_6310, &&inst_7320, &&inst_7169, &&inst_6431, &&inst_4824, &&inst_2968, &&inst_3657, &&inst_7892, &&inst_801, &&inst_2434, &&inst_5829, &&inst_3579, &&inst_5268, &&inst_431, &&inst_8556, &&inst_5337, &&inst_5143, &&inst_9454, &&inst_5521, &&inst_3436, &&inst_485, &&inst_4807, &&inst_8922, &&inst_9089, &&inst_8019, &&inst_572, &&inst_8175, &&inst_6884, &&inst_8436, &&inst_7363, &&inst_460, &&inst_7826, &&inst_3898, &&inst_2421, &&inst_8378, &&inst_2566, &&inst_8653, &&inst_3489, &&inst_4480, &&inst_8542, &&inst_9901, &&inst_6527, &&inst_3760, &&inst_5164, &&inst_163, &&inst_2735, &&inst_9036, &&inst_9424, &&inst_1344, &&inst_9469, &&inst_4937, &&inst_7265, &&inst_6319, &&inst_7336, &&inst_9001, &&inst_1773, &&inst_5347, &&inst_2016, &&inst_8554, &&inst_606, &&inst_7138, &&inst_7519, &&inst_7327, &&inst_2667, &&inst_6521, &&inst_8154, &&inst_3662, &&inst_7803, &&inst_3108, &&inst_7450, &&inst_206, &&inst_3582, &&inst_2586, &&inst_5666, &&inst_6978, &&inst_8896, &&inst_177, &&inst_2705, &&inst_9387, &&inst_320, &&inst_2585, &&inst_2920, &&inst_4958, &&inst_7125, &&inst_1134, &&inst_4530, &&inst_1269, &&inst_2590, &&inst_2524, &&inst_5773, &&inst_6100, &&inst_5448, &&inst_4346, &&inst_622, &&inst_7248, &&inst_4059, &&inst_2582, &&inst_5096, &&inst_7493, &&inst_1373, &&inst_4284, &&inst_6253, &&inst_5449, &&inst_7415, &&inst_8317, &&inst_3095, &&inst_6668, &&inst_9472, &&inst_9840, &&inst_4493, &&inst_3366, &&inst_3747, &&inst_7168, &&inst_5882, &&inst_6645, &&inst_5551, &&inst_7218, &&inst_7560, &&inst_9486, &&inst_1222, &&inst_6127, &&inst_4292, &&inst_7608, &&inst_7753, &&inst_8735, &&inst_6607, &&inst_9261, &&inst_6775, &&inst_4726, &&inst_4085, &&inst_5675, &&inst_383, &&inst_5386, &&inst_7104, &&inst_1299, &&inst_1935, &&inst_9421, &&inst_7108, &&inst_3839, &&inst_7855, &&inst_8161, &&inst_3053, &&inst_875, &&inst_1033, &&inst_7043, &&inst_768, &&inst_369, &&inst_4924, &&inst_8742, &&inst_3594, &&inst_4939, &&inst_6588, &&inst_1257, &&inst_6257, &&inst_8404, &&inst_8101, &&inst_7012, &&inst_7516, &&inst_1680, &&inst_3399, &&inst_2469, &&inst_8097, &&inst_4021, &&inst_3706, &&inst_8644, &&inst_9869, &&inst_7882, &&inst_9517, &&inst_9459, &&inst_6355, &&inst_5648, &&inst_2570, &&inst_4454, &&inst_4499, &&inst_1876, &&inst_2983, &&inst_1212, &&inst_7513, &&inst_4182, &&inst_9882, &&inst_6577, &&inst_4523, &&inst_8737, &&inst_1491, &&inst_3483, &&inst_6625, &&inst_2857, &&inst_8970, &&inst_9337, &&inst_2801, &&inst_9823, &&inst_8792, &&inst_6910, &&inst_8759, &&inst_1082, &&inst_9715, &&inst_815, &&inst_9410, &&inst_8248, &&inst_7306, &&inst_8360, &&inst_8931, &&inst_1488, &&inst_4223, &&inst_8165, &&inst_2349, &&inst_7576, &&inst_9025, &&inst_2921, &&inst_2557, &&inst_1787, &&inst_6994, &&inst_5891, &&inst_780, &&inst_7776, &&inst_9031, &&inst_1729, &&inst_340, &&inst_1353, &&inst_6575, &&inst_8574, &&inst_2005, &&inst_4579, &&inst_7413, &&inst_3228, &&inst_5420, &&inst_5709, &&inst_3808, &&inst_7660, &&inst_6155, &&inst_6140, &&inst_9968, &&inst_3700, &&inst_1597, &&inst_1023, &&inst_6946, &&inst_38, &&inst_7546, &&inst_7192, &&inst_640, &&inst_2337, &&inst_3701, &&inst_6115, &&inst_4170, &&inst_7777, &&inst_5592, &&inst_6582, &&inst_1774, &&inst_9227, &&inst_498, &&inst_584, &&inst_1592, &&inst_1884, &&inst_6922, &&inst_2352, &&inst_6255, &&inst_3677, &&inst_4900, &&inst_5746, &&inst_3812, &&inst_4357, &&inst_5764, &&inst_6135, &&inst_8832, &&inst_3620, &&inst_9719, &&inst_9838, &&inst_1494, &&inst_9582, &&inst_2697, &&inst_7755, &&inst_9567, &&inst_7324, &&inst_4991, &&inst_3301, &&inst_3912, &&inst_7622, &&inst_1343, &&inst_2124, &&inst_7703, &&inst_2779, &&inst_4069, &&inst_5636, &&inst_8852, &&inst_5306, &&inst_8862, &&inst_777, &&inst_9143, &&inst_8932, &&inst_3767, &&inst_5958, &&inst_4020, &&inst_6301, &&inst_6707, &&inst_5926, &&inst_6533, &&inst_9056, &&inst_7228, &&inst_434, &&inst_6031, &&inst_8275, &&inst_5051, &&inst_953, &&inst_9798, &&inst_4355, &&inst_7721, &&inst_8049, &&inst_1348, &&inst_1286, &&inst_1588, &&inst_9932, &&inst_2946, &&inst_6853, &&inst_2584, &&inst_5128, &&inst_836, &&inst_736, &&inst_6024, &&inst_9170, &&inst_9149, &&inst_8871, &&inst_9921, &&inst_334, &&inst_7540, &&inst_4438, &&inst_488, &&inst_9183, &&inst_3234, &&inst_6464, &&inst_5145, &&inst_1599, &&inst_7409, &&inst_9812, &&inst_8408, &&inst_203, &&inst_3007, &&inst_647, &&inst_6963, &&inst_8303, &&inst_7911, &&inst_4825, &&inst_2327, &&inst_4895, &&inst_6022, &&inst_2186, &&inst_1743, &&inst_8319, &&inst_8847, &&inst_2871, &&inst_8373, &&inst_6165, &&inst_2936, &&inst_5406, &&inst_5357, &&inst_3836, &&inst_7449, &&inst_2002, &&inst_4429, &&inst_2039, &&inst_9121, &&inst_6478, &&inst_7055, &&inst_6002, &&inst_5361, &&inst_566, &&inst_2095, &&inst_2638, &&inst_8617, &&inst_9332, &&inst_131, &&inst_2737, &&inst_3246, &&inst_6489, &&inst_3513, &&inst_8541, &&inst_6611, &&inst_7945, &&inst_876, &&inst_7332, &&inst_9455, &&inst_9689, &&inst_9490, &&inst_6480, &&inst_3250, &&inst_2429, &&inst_3214, &&inst_8635, &&inst_6568, &&inst_1880, &&inst_8287, &&inst_8449, &&inst_6610, &&inst_1152, &&inst_5954, &&inst_5889, &&inst_1214, &&inst_6999, &&inst_377, &&inst_7872, &&inst_8168, &&inst_6517, &&inst_3904, &&inst_8868, &&inst_8023, &&inst_6917, &&inst_3137, &&inst_8145, &&inst_9418, &&inst_2454, &&inst_9688, &&inst_6192, &&inst_5047, &&inst_1589, &&inst_9150, &&inst_4162, &&inst_7186, &&inst_8796, &&inst_8775, &&inst_254, &&inst_9, &&inst_3196, &&inst_8240, &&inst_2381, &&inst_1242, &&inst_8682, &&inst_3849, &&inst_1326, &&inst_8659, &&inst_8581, &&inst_9768, &&inst_4632, &&inst_6360, &&inst_8570, &&inst_1512, &&inst_9781};
  int ids[] = {9707, 9987, 6976, 4944, 9681, 9690, 3559, 397, 6470, 195, 7241, 223, 3453, 289, 9136, 8639, 5314, 5082, 2828, 5159, 5631, 6041, 7760, 3614, 9038, 5247, 8245, 1272, 8631, 1391, 2236, 8051, 8026, 5707, 8369, 2014, 9615, 6907, 9478, 5858, 4880, 5098, 7142, 6429, 981, 1314, 257, 8351, 4390, 5042, 1030, 75, 5997, 4950, 6671, 7528, 4001, 8107, 1266, 6786, 9861, 7202, 2133, 9160, 5328, 5609, 5207, 1508, 3151, 4975, 4320, 1443, 9032, 2008, 8974, 929, 7053, 2253, 1084, 3554, 4805, 2836, 8222, 4099, 9745, 4376, 2461, 3997, 8951, 1831, 5811, 9701, 6780, 7226, 7064, 9301, 6750, 5410, 1823, 5784, 4853, 1200, 9577, 2749, 2925, 6929, 8648, 860, 5510, 2506, 9569, 6448, 7987, 4008, 2220, 3521, 9958, 6931, 1397, 5428, 8636, 9799, 1526, 8225, 9302, 3451, 8148, 4524, 6322, 9130, 2958, 7612, 4764, 8197, 3874, 5312, 2167, 5369, 3046, 4299, 1561, 4591, 2939, 4612, 2846, 5539, 5384, 6264, 1603, 8716, 9242, 5588, 9731, 9023, 2212, 3091, 3975, 6177, 3193, 1993, 6601, 3200, 893, 5180, 2316, 6732, 1482, 8753, 6312, 503, 4744, 1206, 3042, 6219, 4373, 6589, 91, 866, 5653, 6823, 9571, 2501, 2344, 586, 5395, 5039, 5270, 3203, 6719, 4457, 5211, 385, 4074, 4190, 9305, 4852, 7828, 6970, 1972, 4075, 8620, 4613, 3160, 8494, 7843, 1628, 7155, 1181, 2339, 6793, 9734, 446, 623, 9682, 5904, 2027, 9686, 6306, 9448, 9826, 6785, 9128, 5339, 5786, 8885, 1372, 8400, 6535, 8233, 9074, 8055, 7562, 8997, 2296, 7220, 5366, 2966, 8778, 6622, 474, 1913, 7106, 2475, 1943, 4038, 8959, 6903, 8672, 596, 9622, 7663, 5290, 885, 761, 1247, 3987, 979, 7058, 6410, 1329, 1421, 1782, 8505, 6761, 3407, 2716, 609, 5417, 5881, 7588, 4628, 5271, 1241, 5960, 9487, 63, 6519, 6357, 8495, 7288, 2203, 4782, 2273, 1025, 1664, 7656, 5561, 2544, 9717, 5241, 6426, 1470, 7601, 2546, 634, 9115, 6285, 1949, 5516, 552, 3111, 1209, 9992, 995, 6404, 1413, 7699, 5456, 8508, 3353, 1656, 4443, 4769, 574, 6121, 3255, 5948, 4066, 9525, 3711, 1825, 6585, 4141, 7119, 8024, 6380, 5910, 1522, 3114, 6092, 1567, 8172, 32, 8918, 8234, 1110, 6765, 7594, 2422, 7573, 6816, 5070, 8214, 5065, 699, 4092, 8860, 4432, 1533, 8797, 3921, 1316, 171, 9800, 6810, 8314, 9952, 1354, 830, 2690, 3945, 9109, 9072, 2644, 7113, 8122, 9184, 5760, 1602, 3678, 8663, 4227, 243, 2858, 4995, 2169, 4823, 5237, 2932, 42, 184, 5485, 4098, 4766, 8257, 997, 68, 3899, 1671, 1556, 8186, 2065, 6399, 7387, 9463, 3932, 2951, 402, 1766, 9623, 5927, 3383, 8134, 9126, 6055, 1657, 6234, 8012, 8045, 5235, 7990, 7709, 4029, 432, 8700, 4281, 2592, 587, 3406, 9442, 5321, 5795, 4987, 1048, 1625, 7057, 7577, 9355, 8909, 1469, 3400, 8286, 2037, 2330, 9048, 8174, 7180, 7045, 2902, 4312, 120, 8414, 3581, 4495, 6624, 9779, 6629, 4733, 2490, 517, 7260, 4447, 1536, 9754, 1364, 4707, 9832, 8593, 8118, 5596, 9394, 9161, 125, 6330, 4936, 1679, 4384, 3984, 2447, 3194, 5616, 3381, 1163, 4300, 7309, 3231, 2, 7115, 6294, 619, 5430, 1207, 8221, 8379, 7484, 1359, 5404, 5851, 4550, 540, 5696, 1767, 757, 4679, 5117, 1325, 5717, 2620, 4232, 8612, 2070, 1256, 4693, 6506, 4031, 5458, 9138, 2057, 1435, 6955, 202, 5971, 3853, 2216, 7717, 6151, 2440, 6212, 478, 7811, 6592, 793, 147, 433, 3699, 5823, 9087, 4356, 6996, 1921, 3509, 5181, 6341, 5612, 6252, 216, 367, 7225, 7770, 3629, 6037, 1044, 8572, 7615, 6289, 6621, 4700, 6363, 5544, 2987, 4935, 3864, 7253, 5228, 2438, 4835, 7347, 9483, 1050, 8250, 9206, 2686, 6894, 5992, 1402, 2180, 8481, 9935, 4360, 2890, 2527, 745, 2935, 6221, 5387, 2277, 1778, 1337, 3883, 1689, 6284, 5964, 7322, 5266, 9658, 2351, 4221, 932, 436, 8790, 3721, 8985, 1963, 8499, 4627, 4686, 4800, 7565, 616, 245, 6891, 1052, 299, 9599, 9825, 3805, 3075, 8297, 7834, 4418, 1612, 982, 3518, 2354, 4181, 6981, 5289, 6598, 9782, 4565, 1695, 5526, 117, 544, 4370, 295, 9508, 1471, 2491, 1673, 1278, 4126, 2268, 2550, 871, 9173, 4982, 4234, 988, 6228, 5686, 149, 2075, 3242, 5918, 6420, 8128, 1730, 3477, 4732, 9291, 6849, 1324, 7172, 6756, 4260, 5016, 2091, 972, 900, 7511, 7632, 7737, 6434, 7065, 93, 795, 6325, 5801, 2563, 3437, 392, 2788, 3503, 8500, 3068, 7637, 6247, 4060, 5500, 2243, 8169, 5486, 3549, 6353, 8509, 4117, 5530, 9899, 3541, 4105, 4076, 2214, 4054, 5511, 7848, 3879, 6296, 8877, 4851, 3040, 8484, 3079, 9956, 1395, 4070, 152, 281, 8063, 357, 8601, 4866, 6936, 7686, 2923, 4945, 1924, 1497, 4361, 3096, 9202, 2157, 4846, 4622, 571, 258, 1941, 913, 3338, 9127, 3254, 5217, 3819, 1055, 2903, 7841, 9163, 9531, 2096, 5466, 6550, 3556, 9910, 1502, 8211, 1849, 5875, 4765, 5619, 4969, 1630, 3419, 2149, 1768, 950, 4770, 139, 5743, 691, 8108, 8196, 2874, 4364, 2730, 2824, 3717, 3616, 1151, 1600, 6883, 2683, 846, 7862, 3827, 6790, 9359, 9155, 8906, 9654, 4625, 6494, 9895, 5226, 7638, 6886, 864, 6082, 2373, 7271, 7206, 2068, 1394, 9695, 8943, 3783, 7863, 5778, 752, 3735, 4718, 8927, 1937, 9930, 2130, 7644, 9181, 1255, 3011, 9260, 7338, 2224, 9915, 5079, 8645, 8265, 3112, 4849, 5255, 2685, 7624, 9824, 7350, 8490, 2992, 7086, 9468, 3302, 945, 9587, 3395, 9004, 5739, 9860, 494, 7414, 2110, 8057, 7868, 5136, 6851, 9971, 6755, 1032, 441, 4508, 6697, 5990, 6701, 9398, 2225, 2888, 2878, 2876, 2496, 8483, 2116, 8009, 8850, 6781, 1280, 7580, 3201, 1752, 3448, 8469, 8638, 2412, 6662, 1714, 4339, 2599, 7996, 7762, 3120, 5915, 706, 4706, 4245, 4218, 1798, 424, 2134, 6848, 234, 9216, 3965, 8338, 4502, 8947, 277, 7973, 8939, 1073, 1725, 172, 7139, 1381, 7988, 5680, 1479, 9007, 7235, 9994, 4597, 5966, 1036, 8258, 8816, 5867, 9735, 3077, 7344, 3292, 522, 6834, 5665, 3370, 9808, 2262, 808, 7147, 8741, 4254, 5776, 5066, 8596, 239, 949, 6440, 4453, 9016, 696, 3423, 9591, 5493, 4813, 6943, 1161, 9933, 1418, 5441, 5202, 233, 267, 9510, 2539, 4653, 3090, 8133, 450, 9590, 2609, 9343, 5498, 2195, 5906, 7937, 4535, 8719, 3936, 5542, 8116, 5794, 7123, 5804, 9140, 5118, 4311, 8095, 2267, 969, 6445, 7718, 4746, 2505, 8656, 8610, 6246, 5017, 8894, 8843, 1926, 9718, 764, 6300, 5499, 7004, 2964, 3412, 901, 7867, 2759, 9998, 8551, 2334, 9928, 9802, 2156, 382, 393, 5043, 7689, 2390, 4030, 1813, 1579, 9834, 2777, 3409, 9806, 8920, 3748, 2985, 8831, 7754, 3673, 4736, 3771, 2561, 4521, 1631, 8982, 1312, 6310, 7320, 7169, 6431, 4824, 2968, 3657, 7892, 801, 2434, 5829, 3579, 5268, 431, 8556, 5337, 5143, 9454, 5521, 3436, 485, 4807, 8922, 9089, 8019, 572, 8175, 6884, 8436, 7363, 460, 7826, 3898, 2421, 8378, 2566, 8653, 3489, 4480, 8542, 9901, 6527, 3760, 5164, 163, 2735, 9036, 9424, 1344, 9469, 4937, 7265, 6319, 7336, 9001, 1773, 5347, 2016, 8554, 606, 7138, 7519, 7327, 2667, 6521, 8154, 3662, 7803, 3108, 7450, 206, 3582, 2586, 5666, 6978, 8896, 177, 2705, 9387, 320, 2585, 2920, 4958, 7125, 1134, 4530, 1269, 2590, 2524, 5773, 6100, 5448, 4346, 622, 7248, 4059, 2582, 5096, 7493, 1373, 4284, 6253, 5449, 7415, 8317, 3095, 6668, 9472, 9840, 4493, 3366, 3747, 7168, 5882, 6645, 5551, 7218, 7560, 9486, 1222, 6127, 4292, 7608, 7753, 8735, 6607, 9261, 6775, 4726, 4085, 5675, 383, 5386, 7104, 1299, 1935, 9421, 7108, 3839, 7855, 8161, 3053, 875, 1033, 7043, 768, 369, 4924, 8742, 3594, 4939, 6588, 1257, 6257, 8404, 8101, 7012, 7516, 1680, 3399, 2469, 8097, 4021, 3706, 8644, 9869, 7882, 9517, 9459, 6355, 5648, 2570, 4454, 4499, 1876, 2983, 1212, 7513, 4182, 9882, 6577, 4523, 8737, 1491, 3483, 6625, 2857, 8970, 9337, 2801, 9823, 8792, 6910, 8759, 1082, 9715, 815, 9410, 8248, 7306, 8360, 8931, 1488, 4223, 8165, 2349, 7576, 9025, 2921, 2557, 1787, 6994, 5891, 780, 7776, 9031, 1729, 340, 1353, 6575, 8574, 2005, 4579, 7413, 3228, 5420, 5709, 3808, 7660, 6155, 6140, 9968, 3700, 1597, 1023, 6946, 38, 7546, 7192, 640, 2337, 3701, 6115, 4170, 7777, 5592, 6582, 1774, 9227, 498, 584, 1592, 1884, 6922, 2352, 6255, 3677, 4900, 5746, 3812, 4357, 5764, 6135, 8832, 3620, 9719, 9838, 1494, 9582, 2697, 7755, 9567, 7324, 4991, 3301, 3912, 7622, 1343, 2124, 7703, 2779, 4069, 5636, 8852, 5306, 8862, 777, 9143, 8932, 3767, 5958, 4020, 6301, 6707, 5926, 6533, 9056, 7228, 434, 6031, 8275, 5051, 953, 9798, 4355, 7721, 8049, 1348, 1286, 1588, 9932, 2946, 6853, 2584, 5128, 836, 736, 6024, 9170, 9149, 8871, 9921, 334, 7540, 4438, 488, 9183, 3234, 6464, 5145, 1599, 7409, 9812, 8408, 203, 3007, 647, 6963, 8303, 7911, 4825, 2327, 4895, 6022, 2186, 1743, 8319, 8847, 2871, 8373, 6165, 2936, 5406, 5357, 3836, 7449, 2002, 4429, 2039, 9121, 6478, 7055, 6002, 5361, 566, 2095, 2638, 8617, 9332, 131, 2737, 3246, 6489, 3513, 8541, 6611, 7945, 876, 7332, 9455, 9689, 9490, 6480, 3250, 2429, 3214, 8635, 6568, 1880, 8287, 8449, 6610, 1152, 5954, 5889, 1214, 6999, 377, 7872, 8168, 6517, 3904, 8868, 8023, 6917, 3137, 8145, 9418, 2454, 9688, 6192, 5047, 1589, 9150, 4162, 7186, 8796, 8775, 254, 9, 3196, 8240, 2381, 1242, 8682, 3849, 1326, 8659, 8581, 9768, 4632, 6360, 8570, 1512, 9781};
  for (int i = 0; i < 1458; i++) {
    if (ids[i] == id) {
      goto *jumptable[i];
    }
  }
  revert_file();
  leave(0);

// nop; @1 @2 5
inst_2016:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 3090 %d %d 5", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b77 3 @1
inst_7513:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_9120") ? getenv("g_9120") : "0") + 3;

	snprintf(output, 200, "./chall $LINENO 7588 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 58 "placebomb_n58"; @1 @2
inst_4182:

	clear_before(line_no);
	if (stack[1] != 58) {

	snprintf(output, 200, "./chall $LINENO 7228 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1278 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @2 18 "placebomb_n18"; @1 @2
inst_8508:

	clear_before(line_no);
	if (stack[1] != 18) {

	snprintf(output, 200, "./chall $LINENO 9161 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5118 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// mul @1 11 @1; @1 @2
inst_6234:

	clear_before(line_no);
	stack[0] = stack[0] * 11;

	snprintf(output, 200, "./chall $LINENO 5904 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b31 60
inst_9510:

	clear_before(line_no);
	tmp = 60;

	snprintf(output, 200, "export g_9074=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9682", line_no + (w_line++));
	leave(0);
// set $b24 65
inst_8107:

	clear_before(line_no);
	tmp = 65;

	snprintf(output, 200, "export g_8262=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5441", line_no + (w_line++));
	leave(0);
// set $b34 3; @1
inst_8775:

	clear_before(line_no);
	tmp = 3;

	snprintf(output, 200, "export g_7534=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 4628 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 15 "placebomb_n15"; @1 @2
inst_9935:

	clear_before(line_no);
	if (stack[1] != 15) {

	snprintf(output, 200, "./chall $LINENO 4281 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 1 $squares
inst_1673:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5145", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b34 @6
inst_3301:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_7534") ? getenv("g_7534") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 11 "getbomb_n11"; @1 @2 @3 @4 @5 @6
inst_6886:

	clear_before(line_no);
	if (stack[4] != 11) {

	snprintf(output, 200, "./chall $LINENO 7288 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5357 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b4 24; @1
inst_9448:

	clear_before(line_no);
	tmp = 24;

	snprintf(output, 200, "export g_4910=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 780 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b17 17; @1
inst_2008:

	clear_before(line_no);
	tmp = 17;

	snprintf(output, 200, "export g_2426=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 1561 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 100000000 $cols
inst_4221:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 584", line_no + (w_line++));
	leave(0);
// add $b80 5 @1
inst_6121:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_4706") ? getenv("g_4706") : "0") + 5;

	snprintf(output, 200, "./chall $LINENO 9291 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 96 "getchar_n96"; @1 @2 @3
inst_6625:

	clear_before(line_no);
	if (stack[2] != 96) {

	snprintf(output, 200, "./chall $LINENO 2951 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5948 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b25 5 @1
inst_8862:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_9999") ? getenv("g_9999") : "0") + 5;

	snprintf(output, 200, "./chall $LINENO 5270 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 8 "getbomb_n8"; @1 @2 @3 @4 @5 @6
inst_4390:

	clear_before(line_no);
	if (stack[4] != 8) {

	snprintf(output, 200, "./chall $LINENO 8639 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2116 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 10 $squares
inst_1522:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2505", line_no + (w_line++));
	leave(0);
// jmp "adj_bomb_calc"; @1 @2 @3 @4 @5
inst_9459:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 2609 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 84 "placebomb_n84"; @1 @2
inst_7169:

	clear_before(line_no);
	if (stack[1] != 84) {

	snprintf(output, 200, "./chall $LINENO 93 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8057 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i61
inst_2344:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i61") ? getenv("i61") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b11 @6
inst_5357:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_4545") ? getenv("g_4545") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 84 "getbomb_n84"; @1 @2 @3 @4 @5 @6
inst_6568:

	clear_before(line_no);
	if (stack[4] != 84) {

	snprintf(output, 200, "./chall $LINENO 6943 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4975 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jg @6 7 "adj_bomb_inc"; @1 @2 @3 @4 @5
inst_9487:

	clear_before(line_no);
	if (stack[5] > 7) {

	snprintf(output, 200, "./chall $LINENO 949 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 234 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @3 30 "getchar_n30"; @1 @2 @3
inst_8483:

	clear_before(line_no);
	if (stack[2] != 30) {

	snprintf(output, 200, "./chall $LINENO 4020 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9582 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b50 75
inst_5361:

	clear_before(line_no);
	tmp = 75;

	snprintf(output, 200, "export g_7833=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6247", line_no + (w_line++));
	leave(0);
// je @1 86 "skip_47"
inst_7811:

	clear_before(line_no);
	if (stack[0] == 86) {
	write_line("./chall $LINENO 498", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 7660", line_no + (w_line++));
	}
	leave(0);
// jne @2 13 "placebomb_n13"; @1 @2
inst_6517:

	clear_before(line_no);
	if (stack[1] != 13) {

	snprintf(output, 200, "./chall $LINENO 7753 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9121 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// mod $b56 7 @1
inst_4782:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_6974") ? getenv("g_6974") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 6645 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 31 "getchar_n31"; @1 @2 @3
inst_4020:

	clear_before(line_no);
	if (stack[2] != 31) {

	snprintf(output, 200, "./chall $LINENO 1082 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4813 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// mod $b49 7 @1
inst_8542:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_4387") ? getenv("g_4387") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 6494 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_8128:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// div @5 9 @6; @1 @2 @3 @4 @5
inst_7115:

	clear_before(line_no);
	stack[5] = stack[4] / 9;

	snprintf(output, 200, "./chall $LINENO 9487 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_5521:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 64 "skip_30"
inst_4939:

	clear_before(line_no);
	if (stack[0] == 64) {
	write_line("./chall $LINENO 233", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 5773", line_no + (w_line++));
	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i84
inst_1443:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i84") ? getenv("i84") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 1000 $cols
inst_2779:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5992", line_no + (w_line++));
	leave(0);
// set $b82 55
inst_2253:

	clear_before(line_no);
	tmp = 55;

	snprintf(output, 200, "export g_4711=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7882", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b56 @6
inst_3255:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_6974") ? getenv("g_6974") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// nop; $c1 @2
inst_1152:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 1200 %d %d", atoi(getenv("g_4968") ? getenv("g_4968") : "0"), stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_5616:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b95 59; @1
inst_5159:

	clear_before(line_no);
	tmp = 59;

	snprintf(output, 200, "export g_5601=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 131 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b8 66; @1
inst_8165:

	clear_before(line_no);
	tmp = 66;

	snprintf(output, 200, "export g_2788=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 5665 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_2857:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 77 "getchar_n77"; @1 @2 @3
inst_5347:

	clear_before(line_no);
	if (stack[2] != 77) {

	snprintf(output, 200, "./chall $LINENO 8931 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6255 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 100000 $squares
inst_6434:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5417", line_no + (w_line++));
	leave(0);
// set $b1 17; @1
inst_8248:

	clear_before(line_no);
	tmp = 17;

	snprintf(output, 200, "export g_8356=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 3234 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i99
inst_2434:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i99") ? getenv("i99") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 100000000 $squares
inst_4339:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6790", line_no + (w_line++));
	leave(0);
// set $b48 94; @1
inst_8175:

	clear_before(line_no);
	tmp = 94;

	snprintf(output, 200, "export g_8883=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 4597 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add @2 @3 @2; @1
inst_2422:

	clear_before(line_no);
	stack[1] = stack[1] + stack[2];

	snprintf(output, 200, "./chall $LINENO 3407 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 1 $rows
inst_7180:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1502", line_no + (w_line++));
	leave(0);
// add $b69 9 @1
inst_3246:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_8228") ? getenv("g_8228") : "0") + 9;

	snprintf(output, 200, "./chall $LINENO 5666 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 60 "placebomb_n60"; @1 @2
inst_91:

	clear_before(line_no);
	if (stack[1] != 60) {

	snprintf(output, 200, "./chall $LINENO 8778 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3200 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @2 2 "placebomb_n2"; @1 @2
inst_2590:

	clear_before(line_no);
	if (stack[1] != 2) {

	snprintf(output, 200, "./chall $LINENO 6155 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5858 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 65 "skip_29"
inst_5458:

	clear_before(line_no);
	if (stack[0] == 65) {
	write_line("./chall $LINENO 3748", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 117", line_no + (w_line++));
	}
	leave(0);
// add $squares 10000000 $squares
inst_1729:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5226", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i34
inst_8414:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i34") ? getenv("i34") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b47 4 @1
inst_9968:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_4136") ? getenv("g_4136") : "0") + 4;

	snprintf(output, 200, "./chall $LINENO 7811 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jg @2 79 "placebomb_a80"; @1 @2
inst_8959:

	clear_before(line_no);
	if (stack[1] > 79) {

	snprintf(output, 200, "./chall $LINENO 9478 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 929 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i49
inst_3120:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i49") ? getenv("i49") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 6 "getbomb_n6"; @1 @2 @3 @4 @5 @6
inst_8404:

	clear_before(line_no);
	if (stack[4] != 6) {

	snprintf(output, 200, "./chall $LINENO 9681 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9016 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b44 85
inst_2373:

	clear_before(line_no);
	tmp = 85;

	snprintf(output, 200, "export g_1900=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9735", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b38 @6
inst_4935:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_5505") ? getenv("g_5505") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 10000 $squares
inst_9812:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4969", line_no + (w_line++));
	leave(0);
// mod $b94 7 @1
inst_9143:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_8506") ? getenv("g_8506") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 1766 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 32 "getchar_n32"; @1 @2 @3
inst_1082:

	clear_before(line_no);
	if (stack[2] != 32) {

	snprintf(output, 200, "./chall $LINENO 8581 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7241 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b65 2 @1
inst_1536:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5070") ? getenv("g_5070") : "0") + 2;

	snprintf(output, 200, "./chall $LINENO 4937 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// nop; $c1 @2
inst_4718:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 9915 %d %d", atoi(getenv("g_4968") ? getenv("g_4968") : "0"), stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b71 2 @1
inst_1025:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_6118") ? getenv("g_6118") : "0") + 2;

	snprintf(output, 200, "./chall $LINENO 3338 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 71 "placebomb_n71"; @1 @2
inst_4732:

	clear_before(line_no);
	if (stack[1] != 71) {

	snprintf(output, 200, "./chall $LINENO 6363 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9163 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b36 @6
inst_8023:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_2497") ? getenv("g_2497") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b79 59; @1
inst_5927:

	clear_before(line_no);
	tmp = 59;

	snprintf(output, 200, "export g_7622=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 997 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 25 "getchar_n25"; @1 @2 @3
inst_7638:

	clear_before(line_no);
	if (stack[2] != 25) {

	snprintf(output, 200, "./chall $LINENO 1391 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4622 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b56 10; @1
inst_981:

	clear_before(line_no);
	tmp = 10;

	snprintf(output, 200, "export g_6974=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 1921 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b97 10; @1
inst_8601:

	clear_before(line_no);
	tmp = 10;

	snprintf(output, 200, "export g_5572=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 68 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 10000000 $squares
inst_4880:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8197", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i32
inst_7241:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i32") ? getenv("i32") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b17 @6
inst_4074:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_2426") ? getenv("g_2426") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 10 $cols
inst_6981:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3549", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b6 @6
inst_9016:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_1827") ? getenv("g_1827") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b48 6 @1
inst_9305:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_8883") ? getenv("g_8883") : "0") + 6;

	snprintf(output, 200, "./chall $LINENO 3582 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add @1 @2 $c2;
inst_8258:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_2431=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9731", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i47
inst_6719:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i47") ? getenv("i47") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 96 "skip_39"
inst_1359:

	clear_before(line_no);
	if (stack[0] == 96) {
	write_line("./chall $LINENO 4706", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 3453", line_no + (w_line++));
	}
	leave(0);
// nop; $c1 @2
inst_32:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 3879 %d %d", atoi(getenv("g_4968") ? getenv("g_4968") : "0"), stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b55 @6
inst_1343:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_3935") ? getenv("g_3935") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i52
inst_1110:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i52") ? getenv("i52") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// mul @1 11 @1; @1 @2
inst_1823:

	clear_before(line_no);
	stack[0] = stack[0] * 11;

	snprintf(output, 200, "./chall $LINENO 2124 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i46
inst_9048:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i46") ? getenv("i46") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 1 $cols
inst_2836:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7108", line_no + (w_line++));
	leave(0);
// add $rows 10000000 $rows
inst_177:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6981", line_no + (w_line++));
	leave(0);
// add $cols 100 $cols
inst_5811:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5047", line_no + (w_line++));
	leave(0);
// set $b95 86
inst_4098:

	clear_before(line_no);
	tmp = 86;

	snprintf(output, 200, "export g_5601=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5395", line_no + (w_line++));
	leave(0);
// je @1 34 "skip_2"
inst_552:

	clear_before(line_no);
	if (stack[0] == 34) {
	write_line("./chall $LINENO 2354", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 4099", line_no + (w_line++));
	}
	leave(0);
// nop; $c0 @2
inst_4117:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4846 %d %d", atoi(getenv("g_8694") ? getenv("g_8694") : "0"), stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add @1 @2 $c1
inst_1023:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_4968=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1073", line_no + (w_line++));
	leave(0);
// jmp "adj_bomb_calc"; @1 @2 @3 @4 @5
inst_2421:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 2609 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 80 "getchar_n80"; @1 @2 @3
inst_1337:

	clear_before(line_no);
	if (stack[2] != 80) {

	snprintf(output, 200, "./chall $LINENO 2506 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5990 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b51 4 @1
inst_2932:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_1277") ? getenv("g_1277") : "0") + 4;

	snprintf(output, 200, "./chall $LINENO 4895 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 10000000 $cols
inst_3965:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3242", line_no + (w_line++));
	leave(0);
// add $cols 10 $cols
inst_2469:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6341", line_no + (w_line++));
	leave(0);
// add $cols 100000 $cols
inst_2749:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3805", line_no + (w_line++));
	leave(0);
// set $b96 24; @1
inst_4851:

	clear_before(line_no);
	tmp = 24;

	snprintf(output, 200, "export g_7748=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 9170 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b21 66; @1
inst_8644:

	clear_before(line_no);
	tmp = 66;

	snprintf(output, 200, "export g_5311=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 5321 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b45 80; @1
inst_9882:

	clear_before(line_no);
	tmp = 80;

	snprintf(output, 200, "export g_5819=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 5096 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 1 $rows
inst_7125:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9824", line_no + (w_line++));
	leave(0);
// jg @2 69 "placebomb_a70"; @1 @2
inst_8174:

	clear_before(line_no);
	if (stack[1] > 69) {

	snprintf(output, 200, "./chall $LINENO 8959 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 91 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b7 10; @1
inst_606:

	clear_before(line_no);
	tmp = 10;

	snprintf(output, 200, "export g_8475=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 9074 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 100000 $squares
inst_4370:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7336", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i43
inst_7413:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i43") ? getenv("i43") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_1561:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "parseloop"
inst_6165:

	clear_before(line_no);
	write_line("./chall $LINENO 7104", line_no + (w_line++));
	leave(0);
// nop; $c1 @2
inst_7045:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4852 %d %d", atoi(getenv("g_4968") ? getenv("g_4968") : "0"), stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 56 "placebomb_n56"; @1 @2
inst_6431:

	clear_before(line_no);
	if (stack[1] != 56) {

	snprintf(output, 200, "./chall $LINENO 7057 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 981 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b53 82
inst_5707:

	clear_before(line_no);
	tmp = 82;

	snprintf(output, 200, "export g_2671=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5596", line_no + (w_line++));
	leave(0);
// jg @6 7 "adj_bomb_inc"; @1 @2 @3 @4 @5
inst_7872:

	clear_before(line_no);
	if (stack[5] > 7) {

	snprintf(output, 200, "./chall $LINENO 949 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8719 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 87 "skip_53"
inst_8982:

	clear_before(line_no);
	if (stack[0] == 87) {
	write_line("./chall $LINENO 9968", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 7718", line_no + (w_line++));
	}
	leave(0);
// jne @3 15 "getchar_n15"; @1 @2 @3
inst_2705:

	clear_before(line_no);
	if (stack[2] != 15) {

	snprintf(output, 200, "./chall $LINENO 9958 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1206 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i33
inst_3250:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i33") ? getenv("i33") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 22 "getbomb_n22"; @1 @2 @3 @4 @5 @6
inst_9398:

	clear_before(line_no);
	if (stack[4] != 22) {

	snprintf(output, 200, "./chall $LINENO 8379 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8154 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b14 7 @1
inst_4969:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_2093") ? getenv("g_2093") : "0") + 7;

	snprintf(output, 200, "./chall $LINENO 3483 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 94 "skip_69"
inst_5666:

	clear_before(line_no);
	if (stack[0] == 94) {
	write_line("./chall $LINENO 6697", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 3412", line_no + (w_line++));
	}
	leave(0);
// jne @3 3 "getchar_n3"; @1 @2 @3
inst_6221:

	clear_before(line_no);
	if (stack[2] != 3) {

	snprintf(output, 200, "./chall $LINENO 609 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7945 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b90 49
inst_8338:

	clear_before(line_no);
	tmp = 49;

	snprintf(output, 200, "export g_4158=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9799", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_8233:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 11 "getchar_n11"; @1 @2 @3
inst_7055:

	clear_before(line_no);
	if (stack[2] != 11) {

	snprintf(output, 200, "./chall $LINENO 7754 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3400 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @3 55 "getchar_n55"; @1 @2 @3
inst_8063:

	clear_before(line_no);
	if (stack[2] != 55) {

	snprintf(output, 200, "./chall $LINENO 6834 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6489 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @2 40 "placebomb_n40"; @1 @2
inst_7519:

	clear_before(line_no);
	if (stack[1] != 40) {

	snprintf(output, 200, "./chall $LINENO 2777 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1767 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @2 76 "placebomb_n76"; @1 @2
inst_6429:

	clear_before(line_no);
	if (stack[1] != 76) {

	snprintf(output, 200, "./chall $LINENO 8593 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6310 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// mod @5 9 @6; @1 @2 @3 @4 @5
inst_441:

	clear_before(line_no);
	stack[5] = stack[4] % 9;

	snprintf(output, 200, "./chall $LINENO 1280 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_6955:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 77 "skip_76"
inst_5449:

	clear_before(line_no);
	if (stack[0] == 77) {
	write_line("./chall $LINENO 8469", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 3945", line_no + (w_line++));
	}
	leave(0);
// jne @5 80 "getbomb_n80"; @1 @2 @3 @4 @5 @6
inst_9901:

	clear_before(line_no);
	if (stack[4] != 80) {

	snprintf(output, 200, "./chall $LINENO 6894 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7562 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// nop; $c0 @2
inst_6910:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 1714 %d %d", atoi(getenv("g_8694") ? getenv("g_8694") : "0"), stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "adj_bomb"; 4 11
inst_6306:

	clear_before(line_no);
	write_line("./chall $LINENO 7776 4 11", line_no + (w_line++));
	leave(0);
// add $cols 100 $cols
inst_1329:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8449", line_no + (w_line++));
	leave(0);
// add $squares 1000 $squares
inst_2133:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8735", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_5619:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mul $c3 13 @2; @1
inst_6645:

	clear_before(line_no);
	stack[1] = atoi(getenv("g_3694") ? getenv("g_3694") : "0") * 13;

	snprintf(output, 200, "./chall $LINENO 9301 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mod $b99 7 @1
inst_9930:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_9888") ? getenv("g_9888") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 4141 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 70 "placebomb_n70"; @1 @2
inst_929:

	clear_before(line_no);
	if (stack[1] != 70) {

	snprintf(output, 200, "./chall $LINENO 4732 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5801 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add @4 1 @4; @1 @2 @3 @4
inst_9932:

	clear_before(line_no);
	stack[3] = stack[3] + 1;

	snprintf(output, 200, "./chall $LINENO 949 %d %d %d %d", stack[0], stack[1], stack[2], stack[3]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i40
inst_8572:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i40") ? getenv("i40") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b54 6 @1
inst_8122:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5793") ? getenv("g_5793") : "0") + 6;

	snprintf(output, 200, "./chall $LINENO 4223 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b73 @6
inst_8997:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_2189") ? getenv("g_2189") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_2268:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 97 "getbomb_n97"; @1 @2 @3 @4 @5 @6
inst_5867:

	clear_before(line_no);
	if (stack[4] != 97) {

	snprintf(output, 200, "./chall $LINENO 8265 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5180 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 1 $squares
inst_3419:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8871", line_no + (w_line++));
	leave(0);
// add $b60 5 @1
inst_3302:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_7980") ? getenv("g_7980") : "0") + 5;

	snprintf(output, 200, "./chall $LINENO 4625 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 65 "placebomb_n65"; @1 @2
inst_2730:

	clear_before(line_no);
	if (stack[1] != 65) {

	snprintf(output, 200, "./chall $LINENO 7450 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7988 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b58 @6
inst_9:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_8511") ? getenv("g_8511") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 100000000 $cols
inst_3898:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4370", line_no + (w_line++));
	leave(0);
// set $b58 80; @1
inst_1278:

	clear_before(line_no);
	tmp = 80;

	snprintf(output, 200, "export g_8511=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 5964 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add @5 8 @5; @1 @2 @3 @4 @5
inst_234:

	clear_before(line_no);
	stack[4] = stack[4] + 8;

	snprintf(output, 200, "./chall $LINENO 2351 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_997:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b96 @6
inst_1849:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_7748") ? getenv("g_7748") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add @1 @2 $c1
inst_3399:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_4968=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5136", line_no + (w_line++));
	leave(0);
// set $b23 52; @1
inst_4443:

	clear_before(line_no);
	tmp = 52;

	snprintf(output, 200, "export g_4184=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 2037 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b75 1 @1
inst_761:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_4412") ? getenv("g_4412") : "0") + 1;

	snprintf(output, 200, "./chall $LINENO 9569 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i9
inst_7516:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i9") ? getenv("i9") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b68 44
inst_9823:

	clear_before_n(line_no, 7);
	tmp = 44;

	snprintf(output, 200, "export g_1117=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4098", line_no + (w_line++));
	leave(0);
// set $b41 66; @1
inst_3984:

	clear_before(line_no);
	tmp = 66;

	snprintf(output, 200, "export g_7941=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 1602 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 10000 $squares
inst_5992:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4706", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i69
inst_6252:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i69") ? getenv("i69") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 20 "placebomb_n20"; @1 @2
inst_5098:

	clear_before(line_no);
	if (stack[1] != 20) {

	snprintf(output, 200, "./chall $LINENO 7493 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4066 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i62
inst_1664:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i62") ? getenv("i62") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// nop; $c1 @2
inst_3700:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 1926 %d %d", atoi(getenv("g_4968") ? getenv("g_4968") : "0"), stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add @1 @2 $c2;
inst_1526:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_2431=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9130", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_239:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 0 "adj_out0"; @1 @2
inst_3079:

	clear_before(line_no);
	if (stack[0] == 0) {

	snprintf(output, 200, "./chall $LINENO 9840 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9001 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @2 88 "placebomb_n88"; @1 @2
inst_2921:

	clear_before(line_no);
	if (stack[1] != 88) {

	snprintf(output, 200, "./chall $LINENO 2014 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3108 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 78 "getbomb_n78"; @1 @2 @3 @4 @5 @6
inst_6151:

	clear_before(line_no);
	if (stack[4] != 78) {

	snprintf(output, 200, "./chall $LINENO 6922 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9394 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 55 "getbomb_n55"; @1 @2 @3 @4 @5 @6
inst_7139:

	clear_before(line_no);
	if (stack[4] != 55) {

	snprintf(output, 200, "./chall $LINENO 860 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1343 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jg @5 69 "getbomb_a70"; @1 @2 @3 @4 @5 @6
inst_8012:

	clear_before(line_no);
	if (stack[4] > 69) {

	snprintf(output, 200, "./chall $LINENO 6765 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6360 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $c2 15
inst_7760:

	clear_before(line_no);
	tmp = 15;

	snprintf(output, 200, "export g_2431=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5544", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_6470:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 10000 $squares
inst_1181:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1084", line_no + (w_line++));
	leave(0);
// jne @5 3 "adj_bomb_inc"; @1 @2 @3 @4
inst_1730:

	clear_before(line_no);
	if (stack[4] != 3) {

	snprintf(output, 200, "./chall $LINENO 949 %d %d %d %d", stack[0], stack[1], stack[2], stack[3]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9932 %d %d %d %d", stack[0], stack[1], stack[2], stack[3]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 84 "skip_15"
inst_1880:

	clear_before(line_no);
	if (stack[0] == 84) {
	write_line("./chall $LINENO 1536", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 2454", line_no + (w_line++));
	}
	leave(0);
// jne @2 3 "placebomb_n3"; @1 @2
inst_6155:

	clear_before(line_no);
	if (stack[1] != 3) {

	snprintf(output, 200, "./chall $LINENO 7580 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8373 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b66 @6
inst_8927:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_1559") ? getenv("g_1559") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 57 "skip_52"
inst_6707:

	clear_before(line_no);
	if (stack[0] == 57) {
	write_line("./chall $LINENO 4535", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 3503", line_no + (w_line++));
	}
	leave(0);
// add $cols 100000 $cols
inst_2236:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 691", line_no + (w_line++));
	leave(0);
// je @1 44 "skip_60"
inst_4625:

	clear_before(line_no);
	if (stack[0] == 44) {
	write_line("./chall $LINENO 761", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 7271", line_no + (w_line++));
	}
	leave(0);
// add $cols 100 $cols
inst_3836:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1884", line_no + (w_line++));
	leave(0);
// exit
inst_7867:

	clear_before(line_no);
	revert_file();
	leave(0);
	leave(0);
// mod $b85 7 @1
inst_9490:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_8360") ? getenv("g_8360") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 696 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b65 10; @1
inst_7988:

	clear_before(line_no);
	tmp = 10;

	snprintf(output, 200, "export g_5070=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 6294 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 64 "getbomb_n64"; @1 @2 @3 @4 @5 @6
inst_9994:

	clear_before(line_no);
	if (stack[4] != 64) {

	snprintf(output, 200, "./chall $LINENO 9686 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7689 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b89 3 @1
inst_4565:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_7572") ? getenv("g_7572") : "0") + 3;

	snprintf(output, 200, "./chall $LINENO 9421 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 10 $rows
inst_9442:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6253", line_no + (w_line++));
	leave(0);
// jne @3 92 "getchar_n92"; @1 @2 @3
inst_4632:

	clear_before(line_no);
	if (stack[2] != 92) {

	snprintf(output, 200, "./chall $LINENO 4765 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9622 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 10000000 $squares
inst_7253:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4693", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i96
inst_5948:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i96") ? getenv("i96") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b43 38; @1
inst_9455:

	clear_before(line_no);
	tmp = 38;

	snprintf(output, 200, "export g_1611=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 8286 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 6 "placebomb_n6"; @1 @2
inst_5499:

	clear_before(line_no);
	if (stack[1] != 6) {

	snprintf(output, 200, "./chall $LINENO 1266 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9798 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// mul $c2 13 @2; @1
inst_8850:

	clear_before(line_no);
	stack[1] = atoi(getenv("g_2431") ? getenv("g_2431") : "0") * 13;

	snprintf(output, 200, "./chall $LINENO 5551 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b59 8 @1
inst_3748:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_3110") ? getenv("g_3110") : "0") + 8;

	snprintf(output, 200, "./chall $LINENO 3395 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 1000 $rows
inst_2273:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2277", line_no + (w_line++));
	leave(0);
// add $cols 1000000 $cols
inst_9590:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5410", line_no + (w_line++));
	leave(0);
// add @5 10 @5; @1 @2 @3 @4 @5
inst_913:

	clear_before(line_no);
	stack[4] = stack[4] + 10;

	snprintf(output, 200, "./chall $LINENO 2609 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 52 "getchar_n52"; @1 @2 @3
inst_9869:

	clear_before(line_no);
	if (stack[2] != 52) {

	snprintf(output, 200, "./chall $LINENO 1222 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1110 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 16 "getbomb_n16"; @1 @2 @3 @4 @5 @6
inst_8909:

	clear_before(line_no);
	if (stack[4] != 16) {

	snprintf(output, 200, "./chall $LINENO 2667 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6312 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b66 10; @1
inst_846:

	clear_before(line_no);
	tmp = 10;

	snprintf(output, 200, "export g_1559=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 5616 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b99 @6
inst_9183:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_9888") ? getenv("g_9888") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 1 $rows
inst_9468:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9332", line_no + (w_line++));
	leave(0);
// add $b8 4 @1
inst_63:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_2788") ? getenv("g_2788") : "0") + 4;

	snprintf(output, 200, "./chall $LINENO 3423 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 2 "getchar_n2"; @1 @2 @3
inst_1831:

	clear_before(line_no);
	if (stack[2] != 2) {

	snprintf(output, 200, "./chall $LINENO 6221 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 42 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b14 58
inst_9799:

	clear_before(line_no);
	tmp = 58;

	snprintf(output, 200, "export g_2093=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6448", line_no + (w_line++));
	leave(0);
// add $cols 10 $cols
inst_3767:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6506", line_no + (w_line++));
	leave(0);
// mod $b79 7 @1
inst_9130:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_7622") ? getenv("g_7622") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 478 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i58
inst_5561:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i58") ? getenv("i58") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b20 57
inst_5954:

	clear_before(line_no);
	tmp = 57;

	snprintf(output, 200, "export g_6630=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5707", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i98
inst_1628:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i98") ? getenv("i98") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jg @2 9 "placebomb_a10"; @1 @2
inst_3053:

	clear_before(line_no);
	if (stack[1] > 9) {

	snprintf(output, 200, "./chall $LINENO 7762 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8257 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jg @3 9 "getchar_a10"; @1 @2 @3
inst_3771:

	clear_before(line_no);
	if (stack[2] > 9) {

	snprintf(output, 200, "./chall $LINENO 1421 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3827 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b74 9 @1
inst_7632:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_8902") ? getenv("g_8902") : "0") + 9;

	snprintf(output, 200, "./chall $LINENO 2557 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b14 @6
inst_6082:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_2093") ? getenv("g_2093") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 70 "getbomb_n70"; @1 @2 @3 @4 @5 @6
inst_2149:

	clear_before(line_no);
	if (stack[4] != 70) {

	snprintf(output, 200, "./chall $LINENO 1052 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1272 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 13 "adj_out13"; @1 @2
inst_9508:

	clear_before(line_no);
	if (stack[0] == 13) {

	snprintf(output, 200, "./chall $LINENO 4718 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3747 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// mul @1 11 @1; @1 @2
inst_1714:

	clear_before(line_no);
	stack[0] = stack[0] * 11;

	snprintf(output, 200, "./chall $LINENO 2527 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mul $c2 13 @2; @1
inst_2638:

	clear_before(line_no);
	stack[1] = atoi(getenv("g_2431") ? getenv("g_2431") : "0") * 13;

	snprintf(output, 200, "./chall $LINENO 1526 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b77 38; @1
inst_2296:

	clear_before(line_no);
	tmp = 38;

	snprintf(output, 200, "export g_9120=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 5266 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 37 "getbomb_n37"; @1 @2 @3 @4 @5 @6
inst_1033:

	clear_before(line_no);
	if (stack[4] != 37) {

	snprintf(output, 200, "./chall $LINENO 4764 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2878 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $rows 100 $rows
inst_3160:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4221", line_no + (w_line++));
	leave(0);
// add $b50 3 @1
inst_431:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_7833") ? getenv("g_7833") : "0") + 3;

	snprintf(output, 200, "./chall $LINENO 5588 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b40 @6
inst_3292:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_8715") ? getenv("g_8715") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b54 59; @1
inst_1949:

	clear_before(line_no);
	tmp = 59;

	snprintf(output, 200, "export g_5793=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 6793 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 12 "getchar_n12"; @1 @2 @3
inst_7754:

	clear_before(line_no);
	if (stack[2] != 12) {

	snprintf(output, 200, "./chall $LINENO 7637 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 257 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @2 22 "placebomb_n22"; @1 @2
inst_8894:

	clear_before(line_no);
	if (stack[1] != 22) {

	snprintf(output, 200, "./chall $LINENO 7737 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9860 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 10000000 $squares
inst_6996:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9695", line_no + (w_line++));
	leave(0);
// add $b33 10 @1
inst_8400:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_9342") ? getenv("g_9342") : "0") + 10;

	snprintf(output, 200, "./chall $LINENO 587 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 80 "placebomb_n80"; @1 @2
inst_6823:

	clear_before(line_no);
	if (stack[1] != 80) {

	snprintf(output, 200, "./chall $LINENO 5217 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 494 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b22 52; @1
inst_9860:

	clear_before(line_no);
	tmp = 52;

	snprintf(output, 200, "export g_3650=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 2801 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 1000000 $squares
inst_8449:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1212", line_no + (w_line++));
	leave(0);
// jne @2 28 "placebomb_n28"; @1 @2
inst_3214:

	clear_before(line_no);
	if (stack[1] != 28) {

	snprintf(output, 200, "./chall $LINENO 6284 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2685 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 83 "getbomb_n83"; @1 @2 @3 @4 @5 @6
inst_2936:

	clear_before(line_no);
	if (stack[4] != 83) {

	snprintf(output, 200, "./chall $LINENO 6568 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5271 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i24
inst_8116:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i24") ? getenv("i24") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 1000 $rows
inst_3678:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5910", line_no + (w_line++));
	leave(0);
// set $b98 61
inst_1787:

	clear_before(line_no);
	tmp = 61;

	snprintf(output, 200, "export g_2643=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5306", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i29
inst_3353:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i29") ? getenv("i29") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i2
inst_42:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i2") ? getenv("i2") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// mod $b95 7 @1
inst_1631:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5601") ? getenv("g_5601") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 8551 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mod $b47 7 @1
inst_2337:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_4136") ? getenv("g_4136") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 7168 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 1000000 $rows
inst_7271:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6420", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_9136:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 100000 $rows
inst_1395:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4679", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i80
inst_5990:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i80") ? getenv("i80") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 1000 $cols
inst_4770:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1344", line_no + (w_line++));
	leave(0);
// jmp "adj_bomb"; 13 41
inst_9343:

	clear_before(line_no);
	write_line("./chall $LINENO 7776 13 41", line_no + (w_line++));
	leave(0);
// add $rows 1000 $rows
inst_1299:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2236", line_no + (w_line++));
	leave(0);
// jmp "placebomb"; @1 @2
inst_3407:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 3053 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 10 $squares
inst_9227:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2932", line_no + (w_line++));
	leave(0);
// set $b99 87; @1
inst_7996:

	clear_before(line_no);
	tmp = 87;

	snprintf(output, 200, "export g_9888=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 932 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 100000 $rows
inst_5384:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3912", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i68
inst_4508:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i68") ? getenv("i68") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 84 "skip_65"
inst_4937:

	clear_before(line_no);
	if (stack[0] == 84) {
	write_line("./chall $LINENO 1212", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 1214", line_no + (w_line++));
	}
	leave(0);
// add $squares 10000000 $squares
inst_1344:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 431", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i41
inst_1354:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i41") ? getenv("i41") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i25
inst_4622:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i25") ? getenv("i25") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// div @5 9 @6; @1 @2 @3 @4 @5
inst_4092:

	clear_before(line_no);
	stack[5] = stack[4] / 9;

	snprintf(output, 200, "./chall $LINENO 8796 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 82 "getbomb_n82"; @1 @2 @3 @4 @5 @6
inst_901:

	clear_before(line_no);
	if (stack[4] != 82) {

	snprintf(output, 200, "./chall $LINENO 2936 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3721 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 86 "getbomb_n86"; @1 @2 @3 @4 @5 @6
inst_2005:

	clear_before(line_no);
	if (stack[4] != 86) {

	snprintf(output, 200, "./chall $LINENO 3409 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9410 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 10000 $squares
inst_3513:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7632", line_no + (w_line++));
	leave(0);
// jne @2 25 "placebomb_n25"; @1 @2
inst_1972:

	clear_before(line_no);
	if (stack[1] != 25) {

	snprintf(output, 200, "./chall $LINENO 596 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4612 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @3 35 "getchar_n35"; @1 @2 @3
inst_6037:

	clear_before(line_no);
	if (stack[2] != 35) {

	snprintf(output, 200, "./chall $LINENO 5117 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8097 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b36 31; @1
inst_6610:

	clear_before(line_no);
	tmp = 31;

	snprintf(output, 200, "export g_2497=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 8672 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 70 "getchar_n70"; @1 @2 @3
inst_8985:

	clear_before(line_no);
	if (stack[2] != 70) {

	snprintf(output, 200, "./chall $LINENO 3228 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2968 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b94 @6
inst_460:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_8506") ? getenv("g_8506") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 8 "adj_out8"; @1 @2
inst_6598:

	clear_before(line_no);
	if (stack[0] == 8) {

	snprintf(output, 200, "./chall $LINENO 9701 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5804 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jl @6 1 "adj_bomb_inc"; @1 @2 @3 @4 @5
inst_1364:

	clear_before(line_no);
	if (stack[5] < 1) {

	snprintf(output, 200, "./chall $LINENO 949 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 517 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @2 26 "placebomb_n26"; @1 @2
inst_596:

	clear_before(line_no);
	if (stack[1] != 26) {

	snprintf(output, 200, "./chall $LINENO 8653 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 764 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b18 89
inst_2327:

	clear_before(line_no);
	tmp = 89;

	snprintf(output, 200, "export g_8904=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5778", line_no + (w_line++));
	leave(0);
// jne @5 92 "getbomb_n92"; @1 @2 @3 @4 @5 @6
inst_2935:

	clear_before(line_no);
	if (stack[4] != 92) {

	snprintf(output, 200, "./chall $LINENO 5648 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2157 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "adj_bomb"; 0 57
inst_5406:

	clear_before(line_no);
	write_line("./chall $LINENO 7776 0 57", line_no + (w_line++));
	leave(0);
// add $squares 1000 $squares
inst_6732:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5428", line_no + (w_line++));
	leave(0);
// add $rows 100000000 $rows
inst_3819:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8148", line_no + (w_line++));
	leave(0);
// jne @3 72 "getchar_n72"; @1 @2 @3
inst_2964:

	clear_before(line_no);
	if (stack[2] != 72) {

	snprintf(output, 200, "./chall $LINENO 9128 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6963 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b24 @6
inst_793:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_8262") ? getenv("g_8262") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jg @5 49 "getbomb_a50"; @1 @2 @3 @4 @5 @6
inst_6946:

	clear_before(line_no);
	if (stack[4] > 49) {

	snprintf(output, 200, "./chall $LINENO 3194 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6264 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "placebomb_out"; @1
inst_5717:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b23 56
inst_1482:

	clear_before(line_no);
	tmp = 56;

	snprintf(output, 200, "export g_4184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5235", line_no + (w_line++));
	leave(0);
// add @1 @2 $c0
inst_5904:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_8694=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4312", line_no + (w_line++));
	leave(0);
// jmp "adj_bomb"; 12 21
inst_1073:

	clear_before(line_no);
	write_line("./chall $LINENO 7776 12 21", line_no + (w_line++));
	leave(0);
// je @1 90 "skip_49"
inst_6588:

	clear_before(line_no);
	if (stack[0] == 90) {
	write_line("./chall $LINENO 7632", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 171", line_no + (w_line++));
	}
	leave(0);
// add $b0 9 @1
inst_8918:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_3624") ? getenv("g_3624") : "0") + 9;

	snprintf(output, 200, "./chall $LINENO 5456 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i73
inst_393:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i73") ? getenv("i73") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_2169:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 1 $rows
inst_7656:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1597", line_no + (w_line++));
	leave(0);
// jne @2 45 "placebomb_n45"; @1 @2
inst_2946:

	clear_before(line_no);
	if (stack[1] != 45) {

	snprintf(output, 200, "./chall $LINENO 1418 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9882 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b26 24; @1
inst_764:

	clear_before(line_no);
	tmp = 24;

	snprintf(output, 200, "export g_3812=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 3579 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 100000 $squares
inst_1679:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9968", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_5266:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b33 @6
inst_488:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_9342") ? getenv("g_9342") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 100000 $rows
inst_866:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4190", line_no + (w_line++));
	leave(0);
// set $b63 39
inst_634:

	clear_before(line_no);
	tmp = 39;

	snprintf(output, 200, "export g_5795=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2524", line_no + (w_line++));
	leave(0);
// mul @3 3 @3; @1 @2 @3
inst_9587:

	clear_before(line_no);
	stack[2] = stack[2] * 3;

	snprintf(output, 200, "./chall $LINENO 4355 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 35 "placebomb_n35"; @1 @2
inst_8700:

	clear_before(line_no);
	if (stack[1] != 35) {

	snprintf(output, 200, "./chall $LINENO 2925 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7565 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $rows 1 $rows
inst_2068:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7843", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_2037:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i16
inst_7260:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i16") ? getenv("i16") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b31 94; @1
inst_2923:

	clear_before(line_no);
	tmp = 94;

	snprintf(output, 200, "export g_9074=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 5500 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b89 @6
inst_8638:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_7572") ? getenv("g_7572") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b84 59; @1
inst_8057:

	clear_before(line_no);
	tmp = 59;

	snprintf(output, 200, "export g_3297=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 8233 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_1937:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 27 "getchar_n27"; @1 @2 @3
inst_1314:

	clear_before(line_no);
	if (stack[2] != 27) {

	snprintf(output, 200, "./chall $LINENO 8554 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6592 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "placebomb_out"; @1
inst_68:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_4292:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 63 "placebomb_n63"; @1 @2
inst_7777:

	clear_before(line_no);
	if (stack[1] != 63) {

	snprintf(output, 200, "./chall $LINENO 8495 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8617 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 32 "skip_4"
inst_6780:

	clear_before(line_no);
	if (stack[0] == 32) {
	write_line("./chall $LINENO 8918", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 9468", line_no + (w_line++));
	}
	leave(0);
// add $b97 3 @1
inst_6246:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5572") ? getenv("g_5572") : "0") + 3;

	snprintf(output, 200, "./chall $LINENO 2002 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 1000 $squares
inst_149:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8051", line_no + (w_line++));
	leave(0);
// add $squares 10000000 $squares
inst_2958:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8469", line_no + (w_line++));
	leave(0);
// jne @2 85 "placebomb_n85"; @1 @2
inst_93:

	clear_before(line_no);
	if (stack[1] != 85) {

	snprintf(output, 200, "./chall $LINENO 2095 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4707 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jg @3 69 "getchar_a70"; @1 @2 @3
inst_3677:

	clear_before(line_no);
	if (stack[2] > 69) {

	snprintf(output, 200, "./chall $LINENO 397 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2412 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $rows 100000000 $rows
inst_9861:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4982", line_no + (w_line++));
	leave(0);
// jg @2 49 "placebomb_a50"; @1 @2
inst_6228:

	clear_before(line_no);
	if (stack[1] > 49) {

	snprintf(output, 200, "./chall $LINENO 7414 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7519 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// nop; @1
inst_4432:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 2475 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b64 3 @1
inst_945:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_2045") ? getenv("g_2045") : "0") + 3;

	snprintf(output, 200, "./chall $LINENO 7511 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b20 24; @1
inst_4066:

	clear_before(line_no);
	tmp = 24;

	snprintf(output, 200, "export g_6630=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 7576 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b55 61
inst_4944:

	clear_before(line_no);
	tmp = 61;

	snprintf(output, 200, "export g_3935=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8556", line_no + (w_line++));
	leave(0);
// add $b3 5 @1
inst_5226:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_3049") ? getenv("g_3049") : "0") + 5;

	snprintf(output, 200, "./chall $LINENO 1470 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 10 $cols
inst_4008:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2133", line_no + (w_line++));
	leave(0);
// add @1 @2 $c0
inst_5709:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_8694=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9768", line_no + (w_line++));
	leave(0);
// je @1 65 "skip_14"
inst_3483:

	clear_before(line_no);
	if (stack[0] == 65) {
	write_line("./chall $LINENO 6296", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 6810", line_no + (w_line++));
	}
	leave(0);
// add $squares 100000000 $squares
inst_7248:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6697", line_no + (w_line++));
	leave(0);
// add $b83 7 @1
inst_4524:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5081") ? getenv("g_5081") : "0") + 7;

	snprintf(output, 200, "./chall $LINENO 1508 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 61 "placebomb_n61"; @1 @2
inst_8778:

	clear_before(line_no);
	if (stack[1] != 61) {

	snprintf(output, 200, "./chall $LINENO 9355 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7220 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $rows 10000 $rows
inst_1935:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9590", line_no + (w_line++));
	leave(0);
// jne @5 17 "getbomb_n17"; @1 @2 @3 @4 @5 @6
inst_2667:

	clear_before(line_no);
	if (stack[4] != 17) {

	snprintf(output, 200, "./chall $LINENO 9150 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4074 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// check $c0 $c1 $c2 $c3 $rows $cols $squares "correct" "incorrect"
inst_8408:

	clear_before(line_no);
	if (atoi(getenv("g_8694") ? getenv("g_8694") : "0") == 1820085546 && atoi(getenv("g_4968") ? getenv("g_4968") : "0") == 1410707190 && atoi(getenv("g_2431") ? getenv("g_2431") : "0") == 972076578 && atoi(getenv("g_3694") ? getenv("g_3694") : "0") == 1718772620 && atoi(getenv("g_7965") ? getenv("g_7965") : "0") == 333333333 && atoi(getenv("g_1829") ? getenv("g_1829") : "0") == 333333333 && atoi(getenv("g_2184") ? getenv("g_2184") : "0") == 333333333) {
		sprintf(output, "export key=%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx", atoi(getenv("g_3624") ? getenv("g_3624") : "0"), atoi(getenv("g_8356") ? getenv("g_8356") : "0"), atoi(getenv("g_2812") ? getenv("g_2812") : "0"), atoi(getenv("g_3049") ? getenv("g_3049") : "0"), atoi(getenv("g_4910") ? getenv("g_4910") : "0"), atoi(getenv("g_2328") ? getenv("g_2328") : "0"), atoi(getenv("g_1827") ? getenv("g_1827") : "0"), atoi(getenv("g_8475") ? getenv("g_8475") : "0"), atoi(getenv("g_2788") ? getenv("g_2788") : "0"), atoi(getenv("g_6560") ? getenv("g_6560") : "0"), atoi(getenv("g_5466") ? getenv("g_5466") : "0"), atoi(getenv("g_4545") ? getenv("g_4545") : "0"), atoi(getenv("g_9560") ? getenv("g_9560") : "0"), atoi(getenv("g_5551") ? getenv("g_5551") : "0"), atoi(getenv("g_2093") ? getenv("g_2093") : "0"), atoi(getenv("g_1484") ? getenv("g_1484") : "0"), atoi(getenv("g_9230") ? getenv("g_9230") : "0"), atoi(getenv("g_2426") ? getenv("g_2426") : "0"), atoi(getenv("g_8904") ? getenv("g_8904") : "0"), atoi(getenv("g_3333") ? getenv("g_3333") : "0"), atoi(getenv("g_6630") ? getenv("g_6630") : "0"), atoi(getenv("g_5311") ? getenv("g_5311") : "0"), atoi(getenv("g_3650") ? getenv("g_3650") : "0"), atoi(getenv("g_4184") ? getenv("g_4184") : "0"), atoi(getenv("g_8262") ? getenv("g_8262") : "0"), atoi(getenv("g_9999") ? getenv("g_9999") : "0"), atoi(getenv("g_3812") ? getenv("g_3812") : "0"), atoi(getenv("g_8508") ? getenv("g_8508") : "0"), atoi(getenv("g_2399") ? getenv("g_2399") : "0"), atoi(getenv("g_9532") ? getenv("g_9532") : "0"), atoi(getenv("g_3862") ? getenv("g_3862") : "0"), atoi(getenv("g_9074") ? getenv("g_9074") : "0"), atoi(getenv("g_7918") ? getenv("g_7918") : "0"), atoi(getenv("g_9342") ? getenv("g_9342") : "0"), atoi(getenv("g_7534") ? getenv("g_7534") : "0"), atoi(getenv("g_5050") ? getenv("g_5050") : "0"), atoi(getenv("g_2497") ? getenv("g_2497") : "0"), atoi(getenv("g_4590") ? getenv("g_4590") : "0"), atoi(getenv("g_5505") ? getenv("g_5505") : "0"), atoi(getenv("g_9283") ? getenv("g_9283") : "0"), atoi(getenv("g_8715") ? getenv("g_8715") : "0"), atoi(getenv("g_7941") ? getenv("g_7941") : "0"), atoi(getenv("g_1804") ? getenv("g_1804") : "0"), atoi(getenv("g_1611") ? getenv("g_1611") : "0"), atoi(getenv("g_1900") ? getenv("g_1900") : "0"), atoi(getenv("g_5819") ? getenv("g_5819") : "0"), atoi(getenv("g_2206") ? getenv("g_2206") : "0"), atoi(getenv("g_4136") ? getenv("g_4136") : "0"), atoi(getenv("g_8883") ? getenv("g_8883") : "0"), atoi(getenv("g_4387") ? getenv("g_4387") : "0"), atoi(getenv("g_7833") ? getenv("g_7833") : "0"), atoi(getenv("g_1277") ? getenv("g_1277") : "0"), atoi(getenv("g_7774") ? getenv("g_7774") : "0"), atoi(getenv("g_2671") ? getenv("g_2671") : "0"), atoi(getenv("g_5793") ? getenv("g_5793") : "0"), atoi(getenv("g_3935") ? getenv("g_3935") : "0"), atoi(getenv("g_6974") ? getenv("g_6974") : "0"), atoi(getenv("g_2988") ? getenv("g_2988") : "0"), atoi(getenv("g_8511") ? getenv("g_8511") : "0"), atoi(getenv("g_3110") ? getenv("g_3110") : "0"), atoi(getenv("g_7980") ? getenv("g_7980") : "0"), atoi(getenv("g_8691") ? getenv("g_8691") : "0"), atoi(getenv("g_7985") ? getenv("g_7985") : "0"), atoi(getenv("g_5795") ? getenv("g_5795") : "0"), atoi(getenv("g_2045") ? getenv("g_2045") : "0"), atoi(getenv("g_5070") ? getenv("g_5070") : "0"), atoi(getenv("g_1559") ? getenv("g_1559") : "0"), atoi(getenv("g_9076") ? getenv("g_9076") : "0"), atoi(getenv("g_1117") ? getenv("g_1117") : "0"), atoi(getenv("g_8228") ? getenv("g_8228") : "0"), atoi(getenv("g_7976") ? getenv("g_7976") : "0"), atoi(getenv("g_6118") ? getenv("g_6118") : "0"), atoi(getenv("g_6199") ? getenv("g_6199") : "0"), atoi(getenv("g_2189") ? getenv("g_2189") : "0"), atoi(getenv("g_8902") ? getenv("g_8902") : "0"), atoi(getenv("g_4412") ? getenv("g_4412") : "0"), atoi(getenv("g_4689") ? getenv("g_4689") : "0"), atoi(getenv("g_9120") ? getenv("g_9120") : "0"), atoi(getenv("g_7345") ? getenv("g_7345") : "0"), atoi(getenv("g_7622") ? getenv("g_7622") : "0"), atoi(getenv("g_4706") ? getenv("g_4706") : "0"));
	write_line(output, line_no + (w_line++));
	write_line("export key=$(echo -n $key | xxd -r -p | sha256sum | awk '{print $1}')", line_no + (w_line++));
	write_line("./chall $LINENO 8234", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 9806", line_no + (w_line++));
	}
	leave(0);
// jne @5 12 "getbomb_n12"; @1 @2 @3 @4 @5 @6
inst_7288:

	clear_before(line_no);
	if (stack[4] != 12) {

	snprintf(output, 200, "./chall $LINENO 6380 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9826 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// exit
inst_2186:

	clear_before(line_no);
	revert_file();
	leave(0);
	leave(0);
// set $b43 36
inst_5596:

	clear_before(line_no);
	tmp = 36;

	snprintf(output, 200, "export g_1611=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8885", line_no + (w_line++));
	leave(0);
// add $rows 1000000 $rows
inst_1373:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7202", line_no + (w_line++));
	leave(0);
// set $b16 17; @1
inst_7803:

	clear_before(line_no);
	tmp = 17;

	snprintf(output, 200, "export g_9230=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 6755 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b87 5 @1
inst_320:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5115") ? getenv("g_5115") : "0") + 5;

	snprintf(output, 200, "./chall $LINENO 6611 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add @1 @2 $c3;
inst_1630:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_3694=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5386", line_no + (w_line++));
	leave(0);
// add $cols 10000000 $cols
inst_4700:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3370", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_8596:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 10000000 $rows
inst_3412:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 147", line_no + (w_line++));
	leave(0);
// add $cols 100000000 $cols
inst_7717:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7608", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b46 @6
inst_9031:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_2206") ? getenv("g_2206") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b72 80; @1
inst_2212:

	clear_before(line_no);
	tmp = 80;

	snprintf(output, 200, "export g_6199=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 2268 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 99 "skip_26"
inst_9181:

	clear_before(line_no);
	if (stack[0] == 99) {
	write_line("./chall $LINENO 3554", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 3160", line_no + (w_line++));
	}
	leave(0);
// jne @5 94 "getbomb_n94"; @1 @2 @3 @4 @5 @6
inst_9036:

	clear_before(line_no);
	if (stack[4] != 94) {

	snprintf(output, 200, "./chall $LINENO 2352 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 460 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $rows 100000 $rows
inst_7718:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5430", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_3699:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i95
inst_2216:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i95") ? getenv("i95") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 78 "placebomb_n78"; @1 @2
inst_9337:

	clear_before(line_no);
	if (stack[1] != 78) {

	snprintf(output, 200, "./chall $LINENO 5927 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1413 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $cols 1000000 $cols
inst_1030:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2939", line_no + (w_line++));
	leave(0);
// set $b0 53
inst_5255:

	clear_before(line_no);
	tmp = 53;

	snprintf(output, 200, "export g_3624=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9910", line_no + (w_line++));
	leave(0);
// set $b3 34
inst_8682:

	clear_before(line_no);
	tmp = 34;

	snprintf(output, 200, "export g_3049=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 369", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b1 @6
inst_5653:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_8356") ? getenv("g_8356") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 18 "getbomb_n18"; @1 @2 @3 @4 @5 @6
inst_9150:

	clear_before(line_no);
	if (stack[4] != 18) {

	snprintf(output, 200, "./chall $LINENO 3091 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9688 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $rows 10000 $rows
inst_243:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1657", line_no + (w_line++));
	leave(0);
// nop
inst_5784:

	clear_before(line_no);
	write_line("./chall $LINENO 4075", line_no + (w_line++));
	leave(0);
// add @1 @2 $c0
inst_8484:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_8694=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6140", line_no + (w_line++));
	leave(0);
// nop
inst_3839:

	clear_before(line_no);
	write_line("./chall $LINENO 5406", line_no + (w_line++));
	leave(0);
// exit
inst_1924:

	clear_before(line_no);
	revert_file();
	leave(0);
	leave(0);
// jne @5 5 "getbomb_n5"; @1 @2 @3 @4 @5 @6
inst_6355:

	clear_before(line_no);
	if (stack[4] != 5) {

	snprintf(output, 200, "./chall $LINENO 8404 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6622 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b4 31
inst_7265:

	clear_before(line_no);
	tmp = 31;

	snprintf(output, 200, "export g_4910=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7338", line_no + (w_line++));
	leave(0);
// add $rows 10000 $rows
inst_6289:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3077", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i3
inst_7945:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i3") ? getenv("i3") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add @1 @2 $c1
inst_4733:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_4968=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8009", line_no + (w_line++));
	leave(0);
// jne @5 91 "getbomb_n91"; @1 @2 @3 @4 @5 @6
inst_522:

	clear_before(line_no);
	if (stack[4] != 91) {

	snprintf(output, 200, "./chall $LINENO 2935 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 289 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $cols 1000 $cols
inst_4679:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7332", line_no + (w_line++));
	leave(0);
// set $b85 65
inst_6671:

	clear_before(line_no);
	tmp = 65;

	snprintf(output, 200, "export g_8360=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8636", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i7
inst_8509:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i7") ? getenv("i7") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jg @6 7 "adj_bomb_inc"; @1 @2 @3 @4 @5
inst_2220:

	clear_before(line_no);
	if (stack[5] > 7) {

	snprintf(output, 200, "./chall $LINENO 949 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7546 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "placebomb_out"; @1
inst_8672:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jg @2 89 "placebomb_a90"; @1 @2
inst_9478:

	clear_before(line_no);
	if (stack[1] > 89) {

	snprintf(output, 200, "./chall $LINENO 6445 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6823 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// nop; 0 0
inst_7104:

	clear_before(line_no);
	write_line("./chall $LINENO 6550 0 0", line_no + (w_line++));
	leave(0);
// mod $b66 7 @1
inst_6135:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_1559") ? getenv("g_1559") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 7699 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_5207:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b92 92
inst_2550:

	clear_before(line_no);
	tmp = 92;

	snprintf(output, 200, "export g_7693=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7622", line_no + (w_line++));
	leave(0);
// jne @3 6 "getchar_n6"; @1 @2 @3
inst_9779:

	clear_before(line_no);
	if (stack[2] != 6) {

	snprintf(output, 200, "./chall $LINENO 8943 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 434 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @2 33 "placebomb_n33"; @1 @2
inst_9987:

	clear_before(line_no);
	if (stack[1] != 33) {

	snprintf(output, 200, "./chall $LINENO 1567 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 795 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @2 30 "placebomb_n30"; @1 @2
inst_7686:

	clear_before(line_no);
	if (stack[1] != 30) {

	snprintf(output, 200, "./chall $LINENO 950 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6786 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b88 8 @1
inst_7172:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_4690") ? getenv("g_4690") : "0") + 8;

	snprintf(output, 200, "./chall $LINENO 9930 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 10000000 $cols
inst_4453:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4339", line_no + (w_line++));
	leave(0);
// jne @5 85 "getbomb_n85"; @1 @2 @3 @4 @5 @6
inst_6943:

	clear_before(line_no);
	if (stack[4] != 85) {

	snprintf(output, 200, "./chall $LINENO 2005 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4360 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "placebomb_out"; @1
inst_8645:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 64 "placebomb_n64"; @1 @2
inst_8495:

	clear_before(line_no);
	if (stack[1] != 64) {

	snprintf(output, 200, "./chall $LINENO 2730 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8169 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @3 23 "getchar_n23"; @1 @2 @3
inst_6929:

	clear_before(line_no);
	if (stack[2] != 23) {

	snprintf(output, 200, "./chall $LINENO 586 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9359 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b57 43
inst_2225:

	clear_before(line_no);
	tmp = 43;

	snprintf(output, 200, "export g_2988=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5070", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b64 @6
inst_7689:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_2045") ? getenv("g_2045") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b40 40
inst_2491:

	clear_before(line_no);
	tmp = 40;

	snprintf(output, 200, "export g_8715=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9952", line_no + (w_line++));
	leave(0);
// jne @5 27 "getbomb_n27"; @1 @2 @3 @4 @5 @6
inst_6781:

	clear_before(line_no);
	if (stack[4] != 27) {

	snprintf(output, 200, "./chall $LINENO 9184 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9654 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b15 75
inst_5306:

	clear_before(line_no);
	tmp = 75;

	snprintf(output, 200, "export g_1484=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5361", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i4
inst_8753:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i4") ? getenv("i4") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b36 8 @1
inst_9695:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_2497") ? getenv("g_2497") : "0") + 8;

	snprintf(output, 200, "./chall $LINENO 1579 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i10
inst_6607:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i10") ? getenv("i10") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b47 52; @1
inst_8716:

	clear_before(line_no);
	tmp = 52;

	snprintf(output, 200, "export g_4136=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 7119 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add @1 @3 @3; @1 @2 @3
inst_3090:

	clear_before(line_no);
	stack[2] = stack[0] + stack[2];

	snprintf(output, 200, "./chall $LINENO 9587 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mod @1 2 @4; @1 @2 @3
inst_9825:

	clear_before(line_no);
	stack[3] = stack[0] % 2;

	snprintf(output, 200, "./chall $LINENO 8297 %d %d %d %d", stack[0], stack[1], stack[2], stack[3]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 64 "skip_77"
inst_7588:

	clear_before(line_no);
	if (stack[0] == 64) {
	write_line("./chall $LINENO 5226", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 7990", line_no + (w_line++));
	}
	leave(0);
// jg @3 59 "getchar_a60"; @1 @2 @3
inst_6884:

	clear_before(line_no);
	if (stack[2] > 59) {

	snprintf(output, 200, "./chall $LINENO 3677 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1055 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// nop; $c0 @2
inst_647:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 5829 %d %d", atoi(getenv("g_8694") ? getenv("g_8694") : "0"), stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 10000000 $cols
inst_7202:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1725", line_no + (w_line++));
	leave(0);
// mod $b82 7 @1
inst_6848:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_4711") ? getenv("g_4711") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 6621 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 16 "getchar_n16"; @1 @2 @3
inst_9958:

	clear_before(line_no);
	if (stack[2] != 16) {

	snprintf(output, 200, "./chall $LINENO 6022 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7260 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 2 "adj_out2"; @1 @2
inst_623:

	clear_before(line_no);
	if (stack[0] == 2) {

	snprintf(output, 200, "./chall $LINENO 4300 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 120 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jl @6 1 "adj_bomb_inc"; @1 @2 @3 @4 @5
inst_8635:

	clear_before(line_no);
	if (stack[5] < 1) {

	snprintf(output, 200, "./chall $LINENO 949 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3201 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $cols 10000 $cols
inst_6399:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2958", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_9525:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b40 8 @1
inst_5164:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_8715") ? getenv("g_8715") : "0") + 8;

	snprintf(output, 200, "./chall $LINENO 2824 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 10 $rows
inst_1774:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4991", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b9 @6
inst_8639:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_6560") ? getenv("g_6560") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mod @5 9 @6; @1 @2 @3 @4 @5
inst_1533:

	clear_before(line_no);
	stack[5] = stack[4] % 9;

	snprintf(output, 200, "./chall $LINENO 8635 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 1000000 $cols
inst_4190:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9160", line_no + (w_line++));
	leave(0);
// set $b17 51
inst_3541:

	clear_before(line_no);
	tmp = 51;

	snprintf(output, 200, "export g_2426=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8656", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_3853:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 35 "skip_5"
inst_9127:

	clear_before(line_no);
	if (stack[0] == 35) {
	write_line("./chall $LINENO 63", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 3904", line_no + (w_line++));
	}
	leave(0);
// jne @3 86 "getchar_n86"; @1 @2 @3
inst_9838:

	clear_before(line_no);
	if (stack[2] != 86) {

	snprintf(output, 200, "./chall $LINENO 2501 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6816 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @3 62 "getchar_n62"; @1 @2 @3
inst_7721:

	clear_before(line_no);
	if (stack[2] != 62) {

	snprintf(output, 200, "./chall $LINENO 4162 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1664 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $cols 10000 $cols
inst_3231:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7106", line_no + (w_line++));
	leave(0);
// set $b71 37
inst_7218:

	clear_before(line_no);
	tmp = 37;

	snprintf(output, 200, "export g_6118=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9658", line_no + (w_line++));
	leave(0);
// add $b93 4 @1
inst_6621:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5063") ? getenv("g_5063") : "0") + 4;

	snprintf(output, 200, "./chall $LINENO 4565 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i75
inst_7320:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i75") ? getenv("i75") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// mul @1 11 @1; @1 @2
inst_3068:

	clear_before(line_no);
	stack[0] = stack[0] * 11;

	snprintf(output, 200, "./chall $LINENO 5709 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 100 $rows
inst_9004:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7235", line_no + (w_line++));
	leave(0);
// jne @2 21 "placebomb_n21"; @1 @2
inst_7493:

	clear_before(line_no);
	if (stack[1] != 21) {

	snprintf(output, 200, "./chall $LINENO 8894 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8644 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b76 10 @1
inst_2195:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_4689") ? getenv("g_4689") : "0") + 10;

	snprintf(output, 200, "./chall $LINENO 5449 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// exit
inst_3747:

	clear_before(line_no);
	revert_file();
	leave(0);
	leave(0);
// set $b83 54
inst_3711:

	clear_before(line_no);
	tmp = 54;

	snprintf(output, 200, "export g_5081=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3509", line_no + (w_line++));
	leave(0);
// add $cols 100000 $cols
inst_2390:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1729", line_no + (w_line++));
	leave(0);
// add $b16 8 @1
inst_4535:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_9230") ? getenv("g_9230") : "0") + 8;

	snprintf(output, 200, "./chall $LINENO 9038 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add @5 9 @5; @1 @2 @3 @4 @5
inst_8719:

	clear_before(line_no);
	stack[4] = stack[4] + 9;

	snprintf(output, 200, "./chall $LINENO 9459 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b25 @6
inst_5960:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_9999") ? getenv("g_9999") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 1000000 $rows
inst_7155:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9571", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i14
inst_1825:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i14") ? getenv("i14") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_6668:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jl @6 1 "adj_bomb_inc"; @1 @2 @3 @4 @5
inst_5017:

	clear_before(line_no);
	if (stack[5] < 1) {

	snprintf(output, 200, "./chall $LINENO 949 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7226 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// echo "Checking... (this may take a while)"
inst_8947:

	clear_before_n(line_no, 102);
	write_line("echo \"Checking... (this may take a while)\"", line_no + (w_line++));
	write_line("./chall $LINENO 6165", line_no + (w_line++));
	leave(0);
// jg @5 89 "getbomb_a90"; @1 @2 @3 @4 @5 @6
inst_5339:

	clear_before(line_no);
	if (stack[4] > 89) {

	snprintf(output, 200, "./chall $LINENO 7560 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9901 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "placebomb_out"; @1
inst_6931:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i26
inst_383:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i26") ? getenv("i26") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 10000 $rows
inst_5794:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5404", line_no + (w_line++));
	leave(0);
// add $squares 100000 $squares
inst_3370:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4535", line_no + (w_line++));
	leave(0);
// add $b84 2 @1
inst_2002:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_3297") ? getenv("g_3297") : "0") + 2;

	snprintf(output, 200, "./chall $LINENO 9143 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 57 "skip_45"
inst_1372:

	clear_before(line_no);
	if (stack[0] == 57) {
	write_line("./chall $LINENO 6426", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 5384", line_no + (w_line++));
	}
	leave(0);
// jl @3 49 "parseloop_invalid"; @1 @2 @3
inst_446:

	clear_before(line_no);
	if (stack[2] < 49) {

	snprintf(output, 200, "./chall $LINENO 2016 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 75 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b34 40
inst_5289:

	clear_before(line_no);
	tmp = 40;

	snprintf(output, 200, "export g_7534=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9895", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b63 @6
inst_1241:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_5795") ? getenv("g_5795") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add @1 @2 $c3;
inst_9301:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_3694=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5784", line_no + (w_line++));
	leave(0);
// jne @5 68 "getbomb_n68"; @1 @2 @3 @4 @5 @6
inst_8360:

	clear_before(line_no);
	if (stack[4] != 68) {

	snprintf(output, 200, "./chall $LINENO 9483 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4849 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i93
inst_8505:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i93") ? getenv("i93") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 39 "skip_71"
inst_3338:

	clear_before(line_no);
	if (stack[0] == 39) {
	write_line("./chall $LINENO 2544", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 382", line_no + (w_line++));
	}
	leave(0);
// add $squares 1000 $squares
inst_9998:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3748", line_no + (w_line++));
	leave(0);
// jne @2 16 "placebomb_n16"; @1 @2
inst_4281:

	clear_before(line_no);
	if (stack[1] != 16) {

	snprintf(output, 200, "./chall $LINENO 2561 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7803 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $cols 10000000 $cols
inst_5404:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6434", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b7 @6
inst_8541:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_8475") ? getenv("g_8475") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b47 82
inst_7622:

	clear_before(line_no);
	tmp = 82;

	snprintf(output, 200, "export g_4136=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2243", line_no + (w_line++));
	leave(0);
// jg @2 59 "placebomb_a60"; @1 @2
inst_7414:

	clear_before(line_no);
	if (stack[1] > 59) {

	snprintf(output, 200, "./chall $LINENO 8174 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3629 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b41 @6
inst_5493:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_7941") ? getenv("g_7941") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b42 3 @1
inst_2544:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_1804") ? getenv("g_1804") : "0") + 3;

	snprintf(output, 200, "./chall $LINENO 5516 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b38 2 @1
inst_8197:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5505") ? getenv("g_5505") : "0") + 2;

	snprintf(output, 200, "./chall $LINENO 7841 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_9718:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 93 "skip_73"
inst_4245:

	clear_before(line_no);
	if (stack[0] == 93) {
	write_line("./chall $LINENO 4126", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 3936", line_no + (w_line++));
	}
	leave(0);
// je @1 82 "skip_48"
inst_3582:

	clear_before(line_no);
	if (stack[0] == 82) {
	write_line("./chall $LINENO 3302", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 1395", line_no + (w_line++));
	}
	leave(0);
// jmp "placebomb_out"; @1
inst_5065:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 38 "placebomb_n38"; @1 @2
inst_5314:

	clear_before(line_no);
	if (stack[1] != 38) {

	snprintf(output, 200, "./chall $LINENO 1326 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4480 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set @2 @3; @1 @2
inst_4060:

	clear_before(line_no);
	stack[1] = stack[2];

	snprintf(output, 200, "./chall $LINENO 6907 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 10000 $rows
inst_3453:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2779", line_no + (w_line++));
	leave(0);
// mod @5 9 @6; @1 @2 @3 @4 @5
inst_2644:

	clear_before(line_no);
	stack[5] = stack[4] % 9;

	snprintf(output, 200, "./chall $LINENO 5609 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b25 10; @1
inst_4612:

	clear_before(line_no);
	tmp = 10;

	snprintf(output, 200, "export g_9999=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 1257 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 88 "getbomb_n88"; @1 @2 @3 @4 @5 @6
inst_7387:

	clear_before(line_no);
	if (stack[4] != 88) {

	snprintf(output, 200, "./chall $LINENO 8638 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8287 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 1000000 $squares
inst_7123:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8400", line_no + (w_line++));
	leave(0);
// add $cols 1000 $cols
inst_752:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5786", line_no + (w_line++));
	leave(0);
// set $b10 43
inst_1151:

	clear_before(line_no);
	tmp = 43;

	snprintf(output, 200, "export g_5466=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2788", line_no + (w_line++));
	leave(0);
// jne @3 68 "getchar_n68"; @1 @2 @3
inst_3518:

	clear_before(line_no);
	if (stack[2] != 68) {

	snprintf(output, 200, "./chall $LINENO 6252 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4508 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b29 @6
inst_3673:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_9532") ? getenv("g_9532") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 10000000 $cols
inst_1597:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5875", line_no + (w_line++));
	leave(0);
// add $b30 1 @1
inst_2505:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_3862") ? getenv("g_3862") : "0") + 1;

	snprintf(output, 200, "./chall $LINENO 4939 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 107 "skip_41"
inst_4356:

	clear_before(line_no);
	if (stack[0] == 107) {
	write_line("./chall $LINENO 4969", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 6322", line_no + (w_line++));
	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b67 @6
inst_8620:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_9076") ? getenv("g_9076") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b76 @6
inst_1048:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_4689") ? getenv("g_4689") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b94 6 @1
inst_1508:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_8506") ? getenv("g_8506") : "0") + 6;

	snprintf(output, 200, "./chall $LINENO 7172 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b86 68
inst_9486:

	clear_before(line_no);
	tmp = 68;

	snprintf(output, 200, "export g_4895=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1151", line_no + (w_line++));
	leave(0);
// jne @2 32 "placebomb_n32"; @1 @2
inst_8145:

	clear_before(line_no);
	if (stack[1] != 32) {

	snprintf(output, 200, "./chall $LINENO 9987 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4807 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b36 39
inst_6883:

	clear_before(line_no);
	tmp = 39;

	snprintf(output, 200, "export g_2497=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9202", line_no + (w_line++));
	leave(0);
// set $b57 31; @1
inst_6589:

	clear_before(line_no);
	tmp = 31;

	snprintf(output, 200, "export g_2988=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 4373 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 83 "placebomb_n83"; @1 @2
inst_9260:

	clear_before(line_no);
	if (stack[1] != 83) {

	snprintf(output, 200, "./chall $LINENO 7169 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 216 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// nop; $c0 @2
inst_7644:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 2461 %d %d", atoi(getenv("g_8694") ? getenv("g_8694") : "0"), stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b80 73; @1
inst_494:

	clear_before(line_no);
	tmp = 73;

	snprintf(output, 200, "export g_4706=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 6470 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// testval for line 624
inst_1348:

	leave(stack[0]);
// jmp "placebomb_out"; @1
inst_4031:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b75 @6
inst_6092:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_4412") ? getenv("g_4412") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $c1 72
inst_1316:

	clear_before(line_no);
	tmp = 72;

	snprintf(output, 200, "export g_4968=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7760", line_no + (w_line++));
	leave(0);
// add $squares 10000 $squares
inst_815:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 233", line_no + (w_line++));
	leave(0);
// jne @5 33 "getbomb_n33"; @1 @2 @3 @4 @5 @6
inst_4054:

	clear_before(line_no);
	if (stack[4] != 33) {

	snprintf(output, 200, "./chall $LINENO 3899 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 488 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 10000 $squares
inst_8831:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3489", line_no + (w_line++));
	leave(0);
// set $b70 52; @1
inst_5801:

	clear_before(line_no);
	tmp = 52;

	snprintf(output, 200, "export g_7976=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 4299 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b54 @6
inst_2584:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_5793") ? getenv("g_5793") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 1 $squares
inst_7058:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9387", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i38
inst_6480:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i38") ? getenv("i38") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b63 7 @1
inst_5369:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5795") ? getenv("g_5795") : "0") + 7;

	snprintf(output, 200, "./chall $LINENO 2690 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_2801:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_9170:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 1 $rows
inst_9023:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3007", line_no + (w_line++));
	leave(0);
// add $squares 100000000 $squares
inst_7608:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2544", line_no + (w_line++));
	leave(0);
// add $rows 1000000 $rows
inst_9577:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5542", line_no + (w_line++));
	leave(0);
// add $cols 100000 $cols
inst_5743:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9517", line_no + (w_line++));
	leave(0);
// div @5 9 @6; @1 @2 @3 @4 @5
inst_3201:

	clear_before(line_no);
	stack[5] = stack[4] / 9;

	snprintf(output, 200, "./chall $LINENO 1778 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b6 17; @1
inst_9798:

	clear_before(line_no);
	tmp = 17;

	snprintf(output, 200, "export g_1827=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 1656 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i76
inst_433:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i76") ? getenv("i76") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// mod $b97 7 @1
inst_6611:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5572") ? getenv("g_5572") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 4311 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b57 3 @1
inst_498:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_2988") ? getenv("g_2988") : "0") + 3;

	snprintf(output, 200, "./chall $LINENO 5337 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b22 @6
inst_8154:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_3650") ? getenv("g_3650") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mod $b93 7 @1
inst_4311:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5063") ? getenv("g_5063") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 8790 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 86 "placebomb_n86"; @1 @2
inst_2095:

	clear_before(line_no);
	if (stack[1] != 86) {

	snprintf(output, 200, "./chall $LINENO 699 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 195 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b64 51
inst_2262:

	clear_before(line_no);
	tmp = 51;

	snprintf(output, 200, "export g_2045=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5289", line_no + (w_line++));
	leave(0);
// add $rows 100000 $rows
inst_171:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9745", line_no + (w_line++));
	leave(0);
// set $b65 82
inst_9682:

	clear_before(line_no);
	tmp = 82;

	snprintf(output, 200, "export g_5070=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4499", line_no + (w_line++));
	leave(0);
// jne @3 5 "adj_bomb_6"; @1 @2 @3 @4 @5
inst_8612:

	clear_before(line_no);
	if (stack[2] != 5) {

	snprintf(output, 200, "./chall $LINENO 988 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6853 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// testval for line 613
inst_706:

	leave(stack[0]);
// jg @3 49 "getchar_a50"; @1 @2 @3
inst_340:

	clear_before(line_no);
	if (stack[2] > 49) {

	snprintf(output, 200, "./chall $LINENO 6884 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3883 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $cols 1 $cols
inst_3912:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6662", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b27 @6
inst_9654:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_8508") ? getenv("g_8508") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b72 5 @1
inst_1084:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_6199") ? getenv("g_6199") : "0") + 5;

	snprintf(output, 200, "./chall $LINENO 7892 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 47 "placebomb_n47"; @1 @2
inst_8026:

	clear_before(line_no);
	if (stack[1] != 47) {

	snprintf(output, 200, "./chall $LINENO 777 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8716 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// mod $b19 7 @1
inst_9731:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_3333") ? getenv("g_3333") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 8850 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b80 @6
inst_7562:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_4706") ? getenv("g_4706") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b23 2 @1
inst_233:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_4184") ? getenv("g_4184") : "0") + 2;

	snprintf(output, 200, "./chall $LINENO 3477 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_254:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b53 52; @1
inst_5486:

	clear_before(line_no);
	tmp = 52;

	snprintf(output, 200, "export g_2671=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 1752 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 50 "getbomb_n50"; @1 @2 @3 @4 @5 @6
inst_5079:

	clear_before(line_no);
	if (stack[4] != 50) {

	snprintf(output, 200, "./chall $LINENO 5268 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8378 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b79 3 @1
inst_2570:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_7622") ? getenv("g_7622") : "0") + 3;

	snprintf(output, 200, "./chall $LINENO 8133 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jg @3 19 "getchar_a20"; @1 @2 @3
inst_1421:

	clear_before(line_no);
	if (stack[2] > 19) {

	snprintf(output, 200, "./chall $LINENO 8250 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4825 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// mul $c2 13 @2; @1
inst_4800:

	clear_before(line_no);
	stack[1] = atoi(getenv("g_2431") ? getenv("g_2431") : "0") * 13;

	snprintf(output, 200, "./chall $LINENO 9992 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jnz @4 "parseloop_place_bomb"; @1 @2 @3
inst_8297:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 1348 %d && ./chall $LINENO 4060 %d %d %d || ./chall $LINENO 2422 %d %d %d", stack[3], stack[0], stack[1], stack[2], stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 76 "skip_75"
inst_9569:

	clear_before(line_no);
	if (stack[0] == 76) {
	write_line("./chall $LINENO 431", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 2057", line_no + (w_line++));
	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b52 @6
inst_2110:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_7774") ? getenv("g_7774") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b44 80; @1
inst_5881:

	clear_before(line_no);
	tmp = 80;

	snprintf(output, 200, "export g_1900=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 9933 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_7576:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 1 $cols
inst_7843:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3419", line_no + (w_line++));
	leave(0);
// jne @3 60 "getchar_n60"; @1 @2 @3
inst_2412:

	clear_before(line_no);
	if (stack[2] != 60) {

	snprintf(output, 200, "./chall $LINENO 6761 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2759 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b9 3 @1
inst_2224:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_6560") ? getenv("g_6560") : "0") + 3;

	snprintf(output, 200, "./chall $LINENO 1469 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 1 "getchar_n1"; @1 @2 @3
inst_4429:

	clear_before(line_no);
	if (stack[2] != 1) {

	snprintf(output, 200, "./chall $LINENO 1831 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5328 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $cols 10 $cols
inst_1993:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1673", line_no + (w_line++));
	leave(0);
// set $b93 73; @1
inst_4824:

	clear_before(line_no);
	tmp = 73;

	snprintf(output, 200, "export g_5063=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 1937 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_1257:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_7828:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 100000 $squares
inst_1494:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 945", line_no + (w_line++));
	leave(0);
// jne @5 53 "getbomb_n53"; @1 @2 @3 @4 @5 @6
inst_3581:

	clear_before(line_no);
	if (stack[4] != 53) {

	snprintf(output, 200, "./chall $LINENO 202 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5926 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i21
inst_5485:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i21") ? getenv("i21") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b7 98
inst_8556:

	clear_before(line_no);
	tmp = 98;

	snprintf(output, 200, "export g_8475=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2327", line_no + (w_line++));
	leave(0);
// je @1 47 "skip_36"
inst_1579:

	clear_before(line_no);
	if (stack[0] == 47) {
	write_line("./chall $LINENO 8051", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 1353", line_no + (w_line++));
	}
	leave(0);
// jmp "adj_bomb"; 3 46
inst_4312:

	clear_before(line_no);
	write_line("./chall $LINENO 7776 3 46", line_no + (w_line++));
	leave(0);
// add $b82 1 @1
inst_8225:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_4711") ? getenv("g_4711") : "0") + 1;

	snprintf(output, 200, "./chall $LINENO 9490 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 91 "placebomb_n91"; @1 @2
inst_7624:

	clear_before(line_no);
	if (stack[1] != 91) {

	snprintf(output, 200, "./chall $LINENO 7862 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2992 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b52 24; @1
inst_2966:

	clear_before(line_no);
	tmp = 24;

	snprintf(output, 200, "export g_7774=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 2096 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b94 35
inst_2490:

	clear_before(line_no);
	tmp = 35;

	snprintf(output, 200, "export g_8506=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9109", line_no + (w_line++));
	leave(0);
// set $b75 59; @1
inst_9715:

	clear_before(line_no);
	tmp = 59;

	snprintf(output, 200, "export g_4412=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 357 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 2 "adj_bomb_3"; @1 @2 @3 @4 @5
inst_2065:

	clear_before(line_no);
	if (stack[2] != 2) {

	snprintf(output, 200, "./chall $LINENO 9971 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2644 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// exit
inst_6464:

	clear_before(line_no);
	revert_file();
	leave(0);
	leave(0);
// set $b88 31; @1
inst_3108:

	clear_before(line_no);
	tmp = 31;

	snprintf(output, 200, "export g_4690=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 9525 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 1 $cols
inst_2737:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9216", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_7119:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 21 "getchar_n21"; @1 @2 @3
inst_8245:

	clear_before(line_no);
	if (stack[2] != 21) {

	snprintf(output, 200, "./chall $LINENO 1603 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5485 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 61 "skip_42"
inst_5516:

	clear_before(line_no);
	if (stack[0] == 61) {
	write_line("./chall $LINENO 4059", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 1935", line_no + (w_line++));
	}
	leave(0);
// add $cols 1000 $cols
inst_9754:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9025", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b2 @6
inst_9808:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_2812") ? getenv("g_2812") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b91 80; @1
inst_2992:

	clear_before(line_no);
	tmp = 80;

	snprintf(output, 200, "export g_6359=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 4292 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 14 "getchar_n14"; @1 @2 @3
inst_4769:

	clear_before(line_no);
	if (stack[2] != 14) {

	snprintf(output, 200, "./chall $LINENO 2705 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1825 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 2 "getbomb_n2"; @1 @2 @3 @4 @5 @6
inst_2871:

	clear_before(line_no);
	if (stack[4] != 2) {

	snprintf(output, 200, "./chall $LINENO 2563 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9808 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 1000000 $squares
inst_8792:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2592", line_no + (w_line++));
	leave(0);
// add $b41 8 @1
inst_3111:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_7941") ? getenv("g_7941") : "0") + 8;

	snprintf(output, 200, "./chall $LINENO 4356 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jg @6 7 "adj_bomb_inc"; @1 @2 @3 @4 @5
inst_8796:

	clear_before(line_no);
	if (stack[5] > 7) {

	snprintf(output, 200, "./chall $LINENO 949 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 913 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b5 @6
inst_6622:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_2328") ? getenv("g_2328") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b13 1 @1
inst_2354:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5551") ? getenv("g_5551") : "0") + 1;

	snprintf(output, 200, "./chall $LINENO 2496 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b15 @6
inst_1050:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_1484") ? getenv("g_1484") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b10 80; @1
inst_8319:

	clear_before(line_no);
	tmp = 80;

	snprintf(output, 200, "export g_5466=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 5717 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 52 "placebomb_n52"; @1 @2
inst_619:

	clear_before(line_no);
	if (stack[1] != 52) {

	snprintf(output, 200, "./chall $LINENO 2983 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2966 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 20 "getbomb_n20"; @1 @2 @3 @4 @5 @6
inst_571:

	clear_before(line_no);
	if (stack[4] != 20) {

	snprintf(output, 200, "./chall $LINENO 4746 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8186 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b31 2 @1
inst_7540:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_9074") ? getenv("g_9074") : "0") + 2;

	snprintf(output, 200, "./chall $LINENO 1044 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mod @5 7 @5; @1 @2 @3 @4
inst_267:

	clear_before(line_no);
	stack[4] = stack[4] % 7;

	snprintf(output, 200, "./chall $LINENO 1730 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 10000 $cols
inst_402:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5675", line_no + (w_line++));
	leave(0);
// je @1 9 "adj_out9"; @1 @2
inst_5804:

	clear_before(line_no);
	if (stack[0] == 9) {

	snprintf(output, 200, "./chall $LINENO 7045 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4070 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "placebomb_out"; @1
inst_4597:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b86 @6
inst_9410:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_4895") ? getenv("g_4895") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_4958:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 11 "adj_out11"; @1 @2
inst_8974:

	clear_before(line_no);
	if (stack[0] == 11) {

	snprintf(output, 200, "./chall $LINENO 3700 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4438 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i27
inst_6592:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i27") ? getenv("i27") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b57 @6
inst_736:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_2988") ? getenv("g_2988") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 22 "getchar_n22"; @1 @2 @3
inst_1603:

	clear_before(line_no);
	if (stack[2] != 22) {

	snprintf(output, 200, "./chall $LINENO 6929 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8659 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $rows 1000000 $rows
inst_5851:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1813", line_no + (w_line++));
	leave(0);
// mod $b46 7 @1
inst_5680:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_2206") ? getenv("g_2206") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 7834 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b15 9 @1
inst_8469:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_1484") ? getenv("g_1484") : "0") + 9;

	snprintf(output, 200, "./chall $LINENO 1880 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add @1 @2 $c3;
inst_6994:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_3694=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8542", line_no + (w_line++));
	leave(0);
// add @1 @2 $c2;
inst_9992:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_2431=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5680", line_no + (w_line++));
	leave(0);
// set $b22 50
inst_2788:

	clear_before(line_no);
	tmp = 50;

	snprintf(output, 200, "export g_3650=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5255", line_no + (w_line++));
	leave(0);
// set $b28 98
inst_9952:

	clear_before(line_no);
	tmp = 98;

	snprintf(output, 200, "export g_2399=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1242", line_no + (w_line++));
	leave(0);
// set $b99 98
inst_2027:

	clear_before(line_no);
	tmp = 98;

	snprintf(output, 200, "export g_9888=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 152", line_no + (w_line++));
	leave(0);
// add $cols 1000 $cols
inst_7987:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 815", line_no + (w_line++));
	leave(0);
// mul @1 11 @1; @1 @2
inst_4852:

	clear_before(line_no);
	stack[0] = stack[0] * 11;

	snprintf(output, 200, "./chall $LINENO 3399 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 100 $cols
inst_5882:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9998", line_no + (w_line++));
	leave(0);
// je @1 43 "skip_19"
inst_9717:

	clear_before(line_no);
	if (stack[0] == 43) {
	write_line("./chall $LINENO 9387", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 9424", line_no + (w_line++));
	}
	leave(0);
// jne @5 95 "getbomb_n95"; @1 @2 @3 @4 @5 @6
inst_2352:

	clear_before(line_no);
	if (stack[4] != 95) {

	snprintf(output, 200, "./chall $LINENO 7344 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8742 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i94
inst_6575:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i94") ? getenv("i94") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i15
inst_1206:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i15") ? getenv("i15") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// mod $b84 7 @1
inst_8631:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_3297") ? getenv("g_3297") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 4524 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_4373:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b63 45; @1
inst_8617:

	clear_before(line_no);
	tmp = 45;

	snprintf(output, 200, "export g_5795=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 5042 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b41 99
inst_8636:

	clear_before(line_no);
	tmp = 99;

	snprintf(output, 200, "export g_7941=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2316", line_no + (w_line++));
	leave(0);
// mod @3 7 @3; @1 @2 @3
inst_4355:

	clear_before(line_no);
	stack[2] = stack[2] % 7;

	snprintf(output, 200, "./chall $LINENO 2429 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 1000 $cols
inst_1813:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7253", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b90 @6
inst_2599:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_4158") ? getenv("g_4158") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_6999:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b27 66; @1
inst_4376:

	clear_before(line_no);
	tmp = 66;

	snprintf(output, 200, "export g_8508=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 6931 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i17
inst_4346:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i17") ? getenv("i17") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_2903:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b68 8 @1
inst_9418:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_1117") ? getenv("g_1117") : "0") + 8;

	snprintf(output, 200, "./chall $LINENO 8499 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i74
inst_6285:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i74") ? getenv("i74") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 100000000 $cols
inst_8148:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5448", line_no + (w_line++));
	leave(0);
// mod @5 9 @6; @1 @2 @3 @4 @5
inst_6410:

	clear_before(line_no);
	stack[5] = stack[4] % 9;

	snprintf(output, 200, "./chall $LINENO 2220 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b12 17; @1
inst_5958:

	clear_before(line_no);
	tmp = 17;

	snprintf(output, 200, "export g_9560=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 8481 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jg @5 59 "getbomb_a60"; @1 @2 @3 @4 @5 @6
inst_3194:

	clear_before(line_no);
	if (stack[4] > 59) {

	snprintf(output, 200, "./chall $LINENO 8012 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5079 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 100 $squares
inst_2939:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8862", line_no + (w_line++));
	leave(0);
// add $rows 10 $rows
inst_8970:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9754", line_no + (w_line++));
	leave(0);
// jne @3 38 "getchar_n38"; @1 @2 @3
inst_7528:

	clear_before(line_no);
	if (stack[2] != 38) {

	snprintf(output, 200, "./chall $LINENO 9531 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6480 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 28 "getbomb_n28"; @1 @2 @3 @4 @5 @6
inst_9184:

	clear_before(line_no);
	if (stack[4] != 28) {

	snprintf(output, 200, "./chall $LINENO 3673 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6701 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 10 $squares
inst_7106:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8408", line_no + (w_line++));
	leave(0);
// jmp "parseloop_inc"; @1 @2
inst_1592:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 2475 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 4 "adj_out4"; @1 @2
inst_4530:

	clear_before(line_no);
	if (stack[0] == 4) {

	snprintf(output, 200, "./chall $LINENO 6910 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9956 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 1000 $squares
inst_4029:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 498", line_no + (w_line++));
	leave(0);
// jne @3 28 "getchar_n28"; @1 @2 @3
inst_8554:

	clear_before(line_no);
	if (stack[2] != 28) {

	snprintf(output, 200, "./chall $LINENO 3353 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2987 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b32 9 @1
inst_5128:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_7918") ? getenv("g_7918") : "0") + 9;

	snprintf(output, 200, "./chall $LINENO 5891 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i57
inst_9781:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i57") ? getenv("i57") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add @1 @2 $c0
inst_5420:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_8694=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1255", line_no + (w_line++));
	leave(0);
// jne @5 24 "getbomb_n24"; @1 @2 @3 @4 @5 @6
inst_7484:

	clear_before(line_no);
	if (stack[4] != 24) {

	snprintf(output, 200, "./chall $LINENO 8951 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 793 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b64 31; @1
inst_8169:

	clear_before(line_no);
	tmp = 31;

	snprintf(output, 200, "export g_2045=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 9802 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i59
inst_3616:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i59") ? getenv("i59") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b78 @6
inst_9394:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_7345") ? getenv("g_7345") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b74 71
inst_9895:

	clear_before(line_no);
	tmp = 71;

	snprintf(output, 200, "export g_8902=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7937", line_no + (w_line++));
	leave(0);
// jg @2 39 "placebomb_a40"; @1 @2
inst_5241:

	clear_before(line_no);
	if (stack[1] > 39) {

	snprintf(output, 200, "./chall $LINENO 6228 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7686 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b0 @6
inst_9472:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_3624") ? getenv("g_3624") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_5964:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b77 61
inst_8436:

	clear_before(line_no);
	tmp = 61;

	snprintf(output, 200, "export g_9120=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3760", line_no + (w_line++));
	leave(0);
// add $squares 10 $squares
inst_9517:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 63", line_no + (w_line++));
	leave(0);
// jne @3 88 "getchar_n88"; @1 @2 @3
inst_2716:

	clear_before(line_no);
	if (stack[2] != 88) {

	snprintf(output, 200, "./chall $LINENO 1589 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8049 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @2 17 "placebomb_n17"; @1 @2
inst_2561:

	clear_before(line_no);
	if (stack[1] != 17) {

	snprintf(output, 200, "./chall $LINENO 8508 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2008 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $rows 10 $rows
inst_6976:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7863", line_no + (w_line++));
	leave(0);
// add $rows 1000 $rows
inst_2874:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3898", line_no + (w_line++));
	leave(0);
// add $rows 10000 $rows
inst_1353:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4493", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b69 @6
inst_9483:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_8228") ? getenv("g_8228") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b20 @6
inst_8186:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_6630") ? getenv("g_6630") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 44 "getchar_n44"; @1 @2 @3
inst_2334:

	clear_before(line_no);
	if (stack[2] != 44) {

	snprintf(output, 200, "./chall $LINENO 3657 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2214 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i70
inst_2968:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i70") ? getenv("i70") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 62 "placebomb_n62"; @1 @2
inst_9355:

	clear_before(line_no);
	if (stack[1] != 62) {

	snprintf(output, 200, "./chall $LINENO 7777 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1488 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b19 10 @1
inst_139:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_3333") ? getenv("g_3333") : "0") + 10;

	snprintf(output, 200, "./chall $LINENO 9717 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 36 "getchar_n36"; @1 @2 @3
inst_5117:

	clear_before(line_no);
	if (stack[2] != 36) {

	snprintf(output, 200, "./chall $LINENO 5915 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7064 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 62 "skip_0"
inst_5456:

	clear_before(line_no);
	if (stack[0] == 62) {
	write_line("./chall $LINENO 8871", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 2068", line_no + (w_line++));
	}
	leave(0);
// set $b1 76
inst_9735:

	clear_before(line_no);
	tmp = 76;

	snprintf(output, 200, "export g_8356=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5043", line_no + (w_line++));
	leave(0);
// add $b34 8 @1
inst_2134:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_7534") ? getenv("g_7534") : "0") + 8;

	snprintf(output, 200, "./chall $LINENO 8045 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 10 $cols
inst_3007:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9155", line_no + (w_line++));
	leave(0);
// mod $b89 7 @1
inst_8551:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_7572") ? getenv("g_7572") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 320 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 33 "skip_9"
inst_1469:

	clear_before(line_no);
	if (stack[0] == 33) {
	write_line("./chall $LINENO 3111", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 6976", line_no + (w_line++));
	}
	leave(0);
// jne @5 71 "getbomb_n71"; @1 @2 @3 @4 @5 @6
inst_1052:

	clear_before(line_no);
	if (stack[4] != 71) {

	snprintf(output, 200, "./chall $LINENO 7327 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2447 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// mul $c2 13 @2; @1
inst_7699:

	clear_before(line_no);
	stack[1] = atoi(getenv("g_2431") ? getenv("g_2431") : "0") * 13;

	snprintf(output, 200, "./chall $LINENO 8258 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 10000000 $rows
inst_5387:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4453", line_no + (w_line++));
	leave(0);
// jne @2 50 "placebomb_n50"; @1 @2
inst_3629:

	clear_before(line_no);
	if (stack[1] != 50) {

	snprintf(output, 200, "./chall $LINENO 3011 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7043 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "placebomb_out"; @1
inst_1602:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mod @5 9 @6; @1 @2 @3 @4 @5
inst_6853:

	clear_before(line_no);
	stack[5] = stack[4] % 9;

	snprintf(output, 200, "./chall $LINENO 801 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b61 6 @1
inst_7409:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_8691") ? getenv("g_8691") : "0") + 6;

	snprintf(output, 200, "./chall $LINENO 5510 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b76 38; @1
inst_6310:

	clear_before(line_no);
	tmp = 38;

	snprintf(output, 200, "export g_4689=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 757 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 64 "getchar_n64"; @1 @2 @3
inst_3559:

	clear_before(line_no);
	if (stack[2] != 64) {

	snprintf(output, 200, "./chall $LINENO 5051 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5918 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @2 66 "placebomb_n66"; @1 @2
inst_7450:

	clear_before(line_no);
	if (stack[1] != 66) {

	snprintf(output, 200, "./chall $LINENO 4085 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 846 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 100000000 $squares
inst_3594:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 761", line_no + (w_line++));
	leave(0);
// jne @2 1 "placebomb_n1"; @1 @2
inst_4950:

	clear_before(line_no);
	if (stack[1] != 1) {

	snprintf(output, 200, "./chall $LINENO 2590 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8248 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 10000000 $squares
inst_5228:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4823", line_no + (w_line++));
	leave(0);
// je @1 80 "skip_11"
inst_9567:

	clear_before(line_no);
	if (stack[0] == 80) {
	write_line("./chall $LINENO 2134", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 1774", line_no + (w_line++));
	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b91 @6
inst_289:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_6359") ? getenv("g_6359") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b54 89
inst_6247:

	clear_before(line_no);
	tmp = 89;

	snprintf(output, 200, "export g_5793=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 540", line_no + (w_line++));
	leave(0);
// add $b28 8 @1
inst_5417:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_2399") ? getenv("g_2399") : "0") + 8;

	snprintf(output, 200, "./chall $LINENO 8161 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i71
inst_6629:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i71") ? getenv("i71") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 100 $rows
inst_5498:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5592", line_no + (w_line++));
	leave(0);
// jg @3 79 "getchar_a80"; @1 @2 @3
inst_397:

	clear_before(line_no);
	if (stack[2] > 79) {

	snprintf(output, 200, "./chall $LINENO 4987 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8985 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// div @5 9 @6; @1 @2 @3 @4 @5
inst_3987:

	clear_before(line_no);
	stack[5] = stack[4] / 9;

	snprintf(output, 200, "./chall $LINENO 5017 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b8 90
inst_6448:

	clear_before(line_no);
	tmp = 90;

	snprintf(output, 200, "export g_2788=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 474", line_no + (w_line++));
	leave(0);
// add $cols 100000000 $cols
inst_4521:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6212", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i12
inst_257:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i12") ? getenv("i12") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 1000000 $cols
inst_4001:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 574", line_no + (w_line++));
	leave(0);
// add $rows 1000 $rows
inst_5773:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7987", line_no + (w_line++));
	leave(0);
// jne @3 45 "getchar_n45"; @1 @2 @3
inst_3657:

	clear_before(line_no);
	if (stack[2] != 45) {

	snprintf(output, 200, "./chall $LINENO 8494 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6970 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b27 39
inst_2846:

	clear_before(line_no);
	tmp = 39;

	snprintf(output, 200, "export g_8508=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2438", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i37
inst_1941:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i37") ? getenv("i37") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 3 "adj_out3"; @1 @2
inst_120:

	clear_before(line_no);
	if (stack[0] == 3) {

	snprintf(output, 200, "./chall $LINENO 4117 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4530 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 1 $squares
inst_9155:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8122", line_no + (w_line++));
	leave(0);
// jne @3 37 "getchar_n37"; @1 @2 @3
inst_5915:

	clear_before(line_no);
	if (stack[2] != 37) {

	snprintf(output, 200, "./chall $LINENO 7528 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1941 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 68 "skip_80"
inst_9291:

	clear_before(line_no);
	if (stack[0] == 68) {
	write_line("./chall $LINENO 2224", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 3819", line_no + (w_line++));
	}
	leave(0);
// add $squares 1 $squares
inst_1512:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2134", line_no + (w_line++));
	leave(0);
// add $rows 100000000 $rows
inst_9149:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9032", line_no + (w_line++));
	leave(0);
// add $b66 5 @1
inst_4693:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_1559") ? getenv("g_1559") : "0") + 5;

	snprintf(output, 200, "./chall $LINENO 5202 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 75 "placebomb_n75"; @1 @2
inst_9072:

	clear_before(line_no);
	if (stack[1] != 75) {

	snprintf(output, 200, "./chall $LINENO 6429 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9715 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b93 56
inst_1773:

	clear_before(line_no);
	tmp = 56;

	snprintf(output, 200, "export g_5063=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5997", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i86
inst_6816:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i86") ? getenv("i86") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b87 @6
inst_7324:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_5115") ? getenv("g_5115") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 96 "placebomb_n96"; @1 @2
inst_5539:

	clear_before(line_no);
	if (stack[1] != 96) {

	snprintf(output, 200, "./chall $LINENO 277 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4851 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b77 @6
inst_1963:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_9120") ? getenv("g_9120") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 100 $squares
inst_574:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1536", line_no + (w_line++));
	leave(0);
// add $squares 100 $squares
inst_5875:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9305", line_no + (w_line++));
	leave(0);
// je @1 6 "adj_out6"; @1 @2
inst_8240:

	clear_before(line_no);
	if (stack[0] == 6) {

	snprintf(output, 200, "./chall $LINENO 7644 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4447 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @3 43 "getchar_n43"; @1 @2 @3
inst_7709:

	clear_before(line_no);
	if (stack[2] != 43) {

	snprintf(output, 200, "./chall $LINENO 2334 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7413 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $cols 100000 $cols
inst_6849:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9812", line_no + (w_line++));
	leave(0);
// jne @2 82 "placebomb_n82"; @1 @2
inst_4357:

	clear_before(line_no);
	if (stack[1] != 82) {

	snprintf(output, 200, "./chall $LINENO 9260 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8847 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b72 85
inst_4284:

	clear_before(line_no);
	tmp = 85;

	snprintf(output, 200, "export g_6199=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5954", line_no + (w_line++));
	leave(0);
// jne @3 81 "getchar_n81"; @1 @2 @3
inst_2506:

	clear_before(line_no);
	if (stack[2] != 81) {

	snprintf(output, 200, "./chall $LINENO 9707 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6978 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 42 "skip_62"
inst_4591:

	clear_before(line_no);
	if (stack[0] == 42) {
	write_line("./chall $LINENO 745", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 7347", line_no + (w_line++));
	}
	leave(0);
// add $rows 10000 $rows
inst_4579:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8055", line_no + (w_line++));
	leave(0);
// add $rows 100000 $rows
inst_7660:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7449", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i53
inst_3717:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i53") ? getenv("i53") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// div @5 9 @6; @1 @2 @3 @4 @5
inst_5366:

	clear_before(line_no);
	stack[5] = stack[4] / 9;

	snprintf(output, 200, "./chall $LINENO 281 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 1000 $squares
inst_6341:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1588", line_no + (w_line++));
	leave(0);
// je @1 63 "skip_70"
inst_7973:

	clear_before(line_no);
	if (stack[0] == 63) {
	write_line("./chall $LINENO 6790", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 5387", line_no + (w_line++));
	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b43 @6
inst_377:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_1611") ? getenv("g_1611") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// nop; $c1 @2
inst_9701:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 1823 %d %d", atoi(getenv("g_4968") ? getenv("g_4968") : "0"), stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mul @1 11 @1; @1 @2
inst_2461:

	clear_before(line_no);
	stack[0] = stack[0] * 11;

	snprintf(output, 200, "./chall $LINENO 8484 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i20
inst_8932:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i20") ? getenv("i20") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i63
inst_3040:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i63") ? getenv("i63") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 90 "skip_72"
inst_7892:

	clear_before(line_no);
	if (stack[0] == 90) {
	write_line("./chall $LINENO 7409", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 9149", line_no + (w_line++));
	}
	leave(0);
// jne @3 7 "getchar_n7"; @1 @2 @3
inst_8943:

	clear_before(line_no);
	if (stack[2] != 7) {

	snprintf(output, 200, "./chall $LINENO 6785 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8509 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b18 73; @1
inst_5118:

	clear_before(line_no);
	tmp = 73;

	snprintf(output, 200, "export g_8904=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 8741 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mul $c2 13 @2; @1
inst_478:

	clear_before(line_no);
	stack[1] = atoi(getenv("g_2431") ? getenv("g_2431") : "0") * 13;

	snprintf(output, 200, "./chall $LINENO 7601 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 1 $squares
inst_1884:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2195", line_no + (w_line++));
	leave(0);
// add $squares 1000000 $squares
inst_8351:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2091", line_no + (w_line++));
	leave(0);
// add $cols 10000 $cols
inst_5910:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1181", line_no + (w_line++));
	leave(0);
// jne @3 41 "getchar_n41"; @1 @2 @3
inst_622:

	clear_before(line_no);
	if (stack[2] != 41) {

	snprintf(output, 200, "./chall $LINENO 864 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1354 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b89 31; @1
inst_2014:

	clear_before(line_no);
	tmp = 31;

	snprintf(output, 200, "export g_7572=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 808 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i89
inst_1589:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i89") ? getenv("i89") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 100000 $cols
inst_3812:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8831", line_no + (w_line++));
	leave(0);
// je @1 106 "skip_28"
inst_8161:

	clear_before(line_no);
	if (stack[0] == 106) {
	write_line("./chall $LINENO 8500", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 2890", line_no + (w_line++));
	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i78
inst_3437:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i78") ? getenv("i78") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 34 "getchar_n34"; @1 @2 @3
inst_3137:

	clear_before(line_no);
	if (stack[2] != 34) {

	snprintf(output, 200, "./chall $LINENO 6037 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8414 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "adj_bomb"; 11 79
inst_8009:

	clear_before(line_no);
	write_line("./chall $LINENO 7776 11 79", line_no + (w_line++));
	leave(0);
// jl @6 1 "adj_bomb_inc"; @1 @2 @3 @4 @5
inst_801:

	clear_before(line_no);
	if (stack[5] < 1) {

	snprintf(output, 200, "./chall $LINENO 949 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7115 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jl @6 1 "adj_bomb_inc"; @1 @2 @3 @4 @5
inst_1778:

	clear_before(line_no);
	if (stack[5] < 1) {

	snprintf(output, 200, "./chall $LINENO 949 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9615 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b29 60
inst_8656:

	clear_before(line_no);
	tmp = 60;

	snprintf(output, 200, "export g_9532=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6671", line_no + (w_line++));
	leave(0);
// add $squares 1 $squares
inst_1689:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3111", line_no + (w_line++));
	leave(0);
// je @1 98 "skip_38"
inst_7841:

	clear_before(line_no);
	if (stack[0] == 98) {
	write_line("./chall $LINENO 5428", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 4579", line_no + (w_line++));
	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i44
inst_2214:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i44") ? getenv("i44") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b42 24; @1
inst_1134:

	clear_before(line_no);
	tmp = 24;

	snprintf(output, 200, "export g_1804=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 6955 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 101 "skip_61"
inst_5510:

	clear_before(line_no);
	if (stack[0] == 101) {
	write_line("./chall $LINENO 1025", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 1373", line_no + (w_line++));
	}
	leave(0);
// jne @3 90 "getchar_n90"; @1 @2 @3
inst_9832:

	clear_before(line_no);
	if (stack[2] != 90) {

	snprintf(output, 200, "./chall $LINENO 4234 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5764 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b42 58
inst_9910:

	clear_before(line_no);
	tmp = 58;

	snprintf(output, 200, "export g_1804=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1773", line_no + (w_line++));
	leave(0);
// add $b46 4 @1
inst_7577:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_2206") ? getenv("g_2206") : "0") + 4;

	snprintf(output, 200, "./chall $LINENO 900 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b33 31; @1
inst_795:

	clear_before(line_no);
	tmp = 31;

	snprintf(output, 200, "export g_9342=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 239 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 42 "getchar_n42"; @1 @2 @3
inst_864:

	clear_before(line_no);
	if (stack[2] != 42) {

	snprintf(output, 200, "./chall $LINENO 7709 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9719 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $rows 10000000 $rows
inst_3783:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 752", line_no + (w_line++));
	leave(0);
// set $b32 37
inst_8211:

	clear_before(line_no);
	tmp = 37;

	snprintf(output, 200, "export g_7918=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2490", line_no + (w_line++));
	leave(0);
// set $b32 24; @1
inst_4807:

	clear_before(line_no);
	tmp = 24;

	snprintf(output, 200, "export g_7918=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 2582 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b16 76
inst_3760:

	clear_before(line_no);
	tmp = 76;

	snprintf(output, 200, "export g_9230=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1787", line_no + (w_line++));
	leave(0);
// jne @3 76 "getchar_n76"; @1 @2 @3
inst_7573:

	clear_before(line_no);
	if (stack[2] != 76) {

	snprintf(output, 200, "./chall $LINENO 5347 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 433 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 100000000 $squares
inst_6212:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 745", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b28 @6
inst_6701:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_2399") ? getenv("g_2399") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 100 $rows
inst_871:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3231", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_2130:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i42
inst_9719:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i42") ? getenv("i42") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 83 "getchar_n83"; @1 @2 @3
inst_6357:

	clear_before(line_no);
	if (stack[2] != 83) {

	snprintf(output, 200, "./chall $LINENO 876 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4936 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $rows 100000 $rows
inst_503:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4008", line_no + (w_line++));
	leave(0);
// jne @2 54 "placebomb_n54"; @1 @2
inst_1036:

	clear_before(line_no);
	if (stack[1] != 54) {

	snprintf(output, 200, "./chall $LINENO 2075 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1949 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $rows 10000000 $rows
inst_3448:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2836", line_no + (w_line++));
	leave(0);
// add $rows 10 $rows
inst_206:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1993", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i55
inst_6489:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i55") ? getenv("i55") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b18 9 @1
inst_1212:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_8904") ? getenv("g_8904") : "0") + 9;

	snprintf(output, 200, "./chall $LINENO 4744 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 81 "getbomb_n81"; @1 @2 @3 @4 @5 @6
inst_6894:

	clear_before(line_no);
	if (stack[4] != 81) {

	snprintf(output, 200, "./chall $LINENO 901 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9921 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getchar"; @1 @2 @1
inst_6550:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 3771 %d %d %d", stack[0], stack[1], stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 1 $squares
inst_5047:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2354", line_no + (w_line++));
	leave(0);
// add @1 @2 $c3;
inst_4924:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_3694=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8369", line_no + (w_line++));
	leave(0);
// jne @5 63 "getbomb_n63"; @1 @2 @3 @4 @5 @6
inst_6330:

	clear_before(line_no);
	if (stack[4] != 63) {

	snprintf(output, 200, "./chall $LINENO 9994 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1241 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $rows 100000 $rows
inst_2620:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3812", line_no + (w_line++));
	leave(0);
// set $b74 59; @1
inst_9126:

	clear_before(line_no);
	tmp = 59;

	snprintf(output, 200, "export g_8902=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 2735 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 3 "getbomb_n3"; @1 @2 @3 @4 @5 @6
inst_2563:

	clear_before(line_no);
	if (stack[4] != 3) {

	snprintf(output, 200, "./chall $LINENO 836 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8737 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i19
inst_172:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i19") ? getenv("i19") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "adj_bomb_calc"; @1 @2 @3 @4 @5
inst_972:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 2609 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 90 "getbomb_n90"; @1 @2 @3 @4 @5 @6
inst_3151:

	clear_before(line_no);
	if (stack[4] != 90) {

	snprintf(output, 200, "./chall $LINENO 522 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2599 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b24 10 @1
inst_2091:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_8262") ? getenv("g_8262") : "0") + 10;

	snprintf(output, 200, "./chall $LINENO 4457 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_932:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_9074:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b13 @6
inst_1625:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_5551") ? getenv("g_5551") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// nop
inst_4075:

	clear_before(line_no);
	write_line("./chall $LINENO 7540", line_no + (w_line++));
	leave(0);
// jmp "adj_bomb_calc"; @1 @2 @3 @4 @5
inst_2381:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 2609 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i50
inst_7206:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i50") ? getenv("i50") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 56 "skip_59"
inst_3395:

	clear_before(line_no);
	if (stack[0] == 56) {
	write_line("./chall $LINENO 4320", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 8574", line_no + (w_line++));
	}
	leave(0);
// je @1 102 "skip_56"
inst_893:

	clear_before(line_no);
	if (stack[0] == 102) {
	write_line("./chall $LINENO 2592", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 7155", line_no + (w_line++));
	}
	leave(0);
// set $b87 57
inst_5441:

	clear_before(line_no);
	tmp = 57;

	snprintf(output, 200, "export g_5115=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8682", line_no + (w_line++));
	leave(0);
// jg @3 39 "getchar_a40"; @1 @2 @3
inst_8832:

	clear_before(line_no);
	if (stack[2] > 39) {

	snprintf(output, 200, "./chall $LINENO 340 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8483 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// store
inst_8275:

	clear_before(line_no);

	char* input = getenv("input");
	if (input == NULL) {
	  revert_file();
	  leave(0);
	}
	int i = 0;
	char output2[10];
	char output3[10];
	while (*input) {
	  snprintf(output2, 10, "i%d", i);
	  snprintf(output3, 10, "%u", (char)(*input));
	  snprintf(output, 200, "export %s=%s", output2, output3);
	  write_line(output, line_no + (w_line++));
	  i++;
	  input++;
	}

	write_line("./chall $LINENO 8947", line_no + (w_line++));
	leave(0);
// add $b26 7 @1
inst_4418:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_3812") ? getenv("g_3812") : "0") + 7;

	snprintf(output, 200, "./chall $LINENO 9181 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mod $b83 7 @1
inst_1766:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5081") ? getenv("g_5081") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 9302 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b74 @6
inst_5795:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_8902") ? getenv("g_8902") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 77 "getbomb_n77"; @1 @2 @3 @4 @5 @6
inst_9242:

	clear_before(line_no);
	if (stack[4] != 77) {

	snprintf(output, 200, "./chall $LINENO 6151 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1963 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b32 @6
inst_5966:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_7918") ? getenv("g_7918") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 21 "getbomb_n21"; @1 @2 @3 @4 @5 @6
inst_4746:

	clear_before(line_no);
	if (stack[4] != 21) {

	snprintf(output, 200, "./chall $LINENO 9398 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7826 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b37 73; @1
inst_7225:

	clear_before(line_no);
	tmp = 73;

	snprintf(output, 200, "export g_4590=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 8168 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b90 38; @1
inst_8303:

	clear_before(line_no);
	tmp = 38;

	snprintf(output, 200, "export g_4158=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 7828 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 0 "getbomb_n0"; @1 @2 @3 @4 @5 @6
inst_8222:

	clear_before(line_no);
	if (stack[4] != 0) {

	snprintf(output, 200, "./chall $LINENO 2180 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9472 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// mod $b91 7 @1
inst_4030:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_6359") ? getenv("g_6359") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 9690 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 10 $squares
inst_9025:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4254", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b49 @6
inst_2566:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_4387") ? getenv("g_4387") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 65 "skip_35"
inst_6903:

	clear_before(line_no);
	if (stack[0] == 65) {
	write_line("./chall $LINENO 7336", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 2874", line_no + (w_line++));
	}
	leave(0);
// jne @2 97 "placebomb_n97"; @1 @2
inst_277:

	clear_before(line_no);
	if (stack[1] != 97) {

	snprintf(output, 200, "./chall $LINENO 2070 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8601 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @2 72 "placebomb_n72"; @1 @2
inst_6363:

	clear_before(line_no);
	if (stack[1] != 72) {

	snprintf(output, 200, "./chall $LINENO 8816 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2212 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 75 "getbomb_n75"; @1 @2 @3 @4 @5 @6
inst_6582:

	clear_before(line_no);
	if (stack[4] != 75) {

	snprintf(output, 200, "./chall $LINENO 1312 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6092 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 73 "getbomb_n73"; @1 @2 @3 @4 @5 @6
inst_4260:

	clear_before(line_no);
	if (stack[4] != 73) {

	snprintf(output, 200, "./chall $LINENO 7363 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8997 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b2 94; @1
inst_5858:

	clear_before(line_no);
	tmp = 94;

	snprintf(output, 200, "export g_2812=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 9718 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jg @5 19 "getbomb_a20"; @1 @2 @3 @4 @5 @6
inst_5739:

	clear_before(line_no);
	if (stack[4] > 19) {

	snprintf(output, 200, "./chall $LINENO 6319 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8019 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 94 "skip_8"
inst_3423:

	clear_before(line_no);
	if (stack[0] == 94) {
	write_line("./chall $LINENO 7513", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 7125", line_no + (w_line++));
	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i83
inst_4936:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i83") ? getenv("i83") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 10000 $cols
inst_3077:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9800", line_no + (w_line++));
	leave(0);
// add @1 @2 $c3;
inst_8610:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_3694=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4782", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b21 @6
inst_7826:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_5311") ? getenv("g_5311") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b39 3 @1
inst_544:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_9283") ? getenv("g_9283") : "0") + 3;

	snprintf(output, 200, "./chall $LINENO 1359 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mul @2 9 @2; @1 @2
inst_6907:

	clear_before(line_no);
	stack[1] = stack[1] * 9;

	snprintf(output, 200, "./chall $LINENO 1592 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 62 "getbomb_n62"; @1 @2 @3 @4 @5 @6
inst_6325:

	clear_before(line_no);
	if (stack[4] != 62) {

	snprintf(output, 200, "./chall $LINENO 6330 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5143 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b75 75
inst_485:

	clear_before(line_no);
	tmp = 75;

	snprintf(output, 200, "export g_4412=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4726", line_no + (w_line++));
	leave(0);
// jne @2 77 "placebomb_n77"; @1 @2
inst_8593:

	clear_before(line_no);
	if (stack[1] != 77) {

	snprintf(output, 200, "./chall $LINENO 9337 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2296 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "adj_bomb"; 2 49
inst_1255:

	clear_before(line_no);
	write_line("./chall $LINENO 7776 2 49", line_no + (w_line++));
	leave(0);
// jne @3 87 "getchar_n87"; @1 @2 @3
inst_2501:

	clear_before(line_no);
	if (stack[2] != 87) {

	snprintf(output, 200, "./chall $LINENO 2716 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3808 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @3 71 "getchar_n71"; @1 @2 @3
inst_3228:

	clear_before(line_no);
	if (stack[2] != 71) {

	snprintf(output, 200, "./chall $LINENO 2964 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6629 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "parseloop_ok_char"; @1 @2 @3
inst_7415:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 9825 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 40 "getbomb_n40"; @1 @2 @3 @4 @5 @6
inst_6264:

	clear_before(line_no);
	if (stack[4] != 40) {

	snprintf(output, 200, "./chall $LINENO 5760 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3292 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @3 50 "getchar_n50"; @1 @2 @3
inst_1055:

	clear_before(line_no);
	if (stack[2] != 50) {

	snprintf(output, 200, "./chall $LINENO 9087 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7206 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// mul $c3 13 @2; @1
inst_8172:

	clear_before(line_no);
	stack[1] = atoi(getenv("g_3694") ? getenv("g_3694") : "0") * 13;

	snprintf(output, 200, "./chall $LINENO 7004 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "adj_bomb_calc"; @1 @2 @3 @4 @5
inst_245:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 2609 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b69 85
inst_7882:

	clear_before(line_no);
	tmp = 85;

	snprintf(output, 200, "export g_8228=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3874", line_no + (w_line++));
	leave(0);
// add $b96 6 @1
inst_6533:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_7748") ? getenv("g_7748") : "0") + 6;

	snprintf(output, 200, "./chall $LINENO 4021 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "adj_bomb"; 1 24
inst_9768:

	clear_before(line_no);
	write_line("./chall $LINENO 7776 1 24", line_no + (w_line++));
	leave(0);
// add $squares 1000000 $squares
inst_3549:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 544", line_no + (w_line++));
	leave(0);
// add $b92 3 @1
inst_5971:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_7693") ? getenv("g_7693") : "0") + 3;

	snprintf(output, 200, "./chall $LINENO 6848 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b49 84
inst_3864:

	clear_before(line_no);
	tmp = 84;

	snprintf(output, 200, "export g_4387=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 485", line_no + (w_line++));
	leave(0);
// add $b49 6 @1
inst_1588:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_4387") ? getenv("g_4387") : "0") + 6;

	snprintf(output, 200, "./chall $LINENO 6588 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// nop; @1 @2 0 0 @2
inst_7776:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 9623 %d %d 0 0 %d", stack[0], stack[1], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 17 "getchar_n17"; @1 @2 @3
inst_6022:

	clear_before(line_no);
	if (stack[2] != 17) {

	snprintf(output, 200, "./chall $LINENO 3096 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4346 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "placebomb_out"; @1
inst_3234:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b20 9 @1
inst_3554:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_6630") ? getenv("g_6630") : "0") + 9;

	snprintf(output, 200, "./chall $LINENO 2267 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i92
inst_9622:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i92") ? getenv("i92") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 1 $cols
inst_7863:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1689", line_no + (w_line++));
	leave(0);
// set $b60 45; @1
inst_3200:

	clear_before(line_no);
	tmp = 45;

	snprintf(output, 200, "export g_7980=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 6668 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_8741:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 100000000 $rows
inst_7990:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2390", line_no + (w_line++));
	leave(0);
// jne @2 0 "placebomb_n0"; @1 @2
inst_8257:

	clear_before(line_no);
	if (stack[1] != 0) {

	snprintf(output, 200, "./chall $LINENO 4950 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 982 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i67
inst_8843:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i67") ? getenv("i67") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 88 "skip_44"
inst_5526:

	clear_before(line_no);
	if (stack[0] == 88) {
	write_line("./chall $LINENO 5164", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 243", line_no + (w_line++));
	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i11
inst_3400:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i11") ? getenv("i11") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 1000 $squares
inst_7113:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8500", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_8168:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 100 $cols
inst_9571:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8792", line_no + (w_line++));
	leave(0);
// add @1 @2 $c2;
inst_5551:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_2431=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9007", line_no + (w_line++));
	leave(0);
// jne @2 46 "placebomb_n46"; @1 @2
inst_1418:

	clear_before(line_no);
	if (stack[1] != 46) {

	snprintf(output, 200, "./chall $LINENO 8026 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 640 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jg @2 29 "placebomb_a30"; @1 @2
inst_1768:

	clear_before(line_no);
	if (stack[1] > 29) {

	snprintf(output, 200, "./chall $LINENO 5241 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5098 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 61 "getbomb_n61"; @1 @2 @3 @4 @5 @6
inst_295:

	clear_before(line_no);
	if (stack[4] != 61) {

	snprintf(output, 200, "./chall $LINENO 6325 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4995 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @3 3 "adj_bomb_4"; @1 @2 @3 @4 @5
inst_9971:

	clear_before(line_no);
	if (stack[2] != 3) {

	snprintf(output, 200, "./chall $LINENO 3366 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5746 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @2 11 "placebomb_n11"; @1 @2
inst_9599:

	clear_before(line_no);
	if (stack[1] != 11) {

	snprintf(output, 200, "./chall $LINENO 6535 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7615 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b90 7 @1
inst_696:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_4158") ? getenv("g_4158") : "0") + 7;

	snprintf(output, 200, "./chall $LINENO 5971 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b70 61
inst_4227:

	clear_before(line_no);
	tmp = 61;

	snprintf(output, 200, "export g_7976=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9206", line_no + (w_line++));
	leave(0);
// add $cols 10000 $cols
inst_1435:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5228", line_no + (w_line++));
	leave(0);
// je @1 46 "skip_66"
inst_5202:

	clear_before(line_no);
	if (stack[0] == 46) {
	write_line("./chall $LINENO 5889", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 3783", line_no + (w_line++));
	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i87
inst_3808:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i87") ? getenv("i87") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b79 @6
inst_6922:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_7622") ? getenv("g_7622") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b23 @6
inst_2888:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_4184") ? getenv("g_4184") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 1 $rows
inst_4866:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3112", line_no + (w_line++));
	leave(0);
// add $b12 7 @1
inst_5889:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_9560") ? getenv("g_9560") : "0") + 7;

	snprintf(output, 200, "./chall $LINENO 3042 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b60 39
inst_8024:

	clear_before(line_no);
	tmp = 39;

	snprintf(output, 200, "export g_7980=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2262", line_no + (w_line++));
	leave(0);
// jne @2 34 "placebomb_n34"; @1 @2
inst_1567:

	clear_before(line_no);
	if (stack[1] != 34) {

	snprintf(output, 200, "./chall $LINENO 8700 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8775 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b45 @6
inst_875:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_5819") ? getenv("g_5819") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_780:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jg @5 29 "getbomb_a30"; @1 @2 @3 @4 @5 @6
inst_6319:

	clear_before(line_no);
	if (stack[4] > 29) {

	snprintf(output, 200, "./chall $LINENO 3921 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 571 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 10 $squares
inst_1876:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8918", line_no + (w_line++));
	leave(0);
// add $cols 10 $cols
inst_8134:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7058", line_no + (w_line++));
	leave(0);
// jne @2 31 "placebomb_n31"; @1 @2
inst_950:

	clear_before(line_no);
	if (stack[1] != 31) {

	snprintf(output, 200, "./chall $LINENO 8145 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2923 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @6 0 "getbomb_out0"; @1 @2 @3 @4 @5
inst_6775:

	clear_before(line_no);
	if (stack[5] == 0) {

	snprintf(output, 200, "./chall $LINENO 267 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6464 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// mul @1 11 @1; @1 @2
inst_3879:

	clear_before(line_no);
	stack[0] = stack[0] * 11;

	snprintf(output, 200, "./chall $LINENO 6521 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mul $c3 13 @2; @1
inst_3193:

	clear_before(line_no);
	stack[1] = atoi(getenv("g_3694") ? getenv("g_3694") : "0") * 13;

	snprintf(output, 200, "./chall $LINENO 6994 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 51 "getbomb_n51"; @1 @2 @3 @4 @5 @6
inst_5268:

	clear_before(line_no);
	if (stack[4] != 51) {

	snprintf(output, 200, "./chall $LINENO 8759 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7755 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// nop; $c0 @2
inst_3095:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4627 %d %d", atoi(getenv("g_8694") ? getenv("g_8694") : "0"), stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 10 $rows
inst_2454:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4001", line_no + (w_line++));
	leave(0);
// set $b81 94; @1
inst_5466:

	clear_before(line_no);
	tmp = 94;

	snprintf(output, 200, "export g_4635=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 2903 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 1 "getbomb_n1"; @1 @2 @3 @4 @5 @6
inst_2180:

	clear_before(line_no);
	if (stack[4] != 1) {

	snprintf(output, 200, "./chall $LINENO 2871 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5653 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// mul @1 11 @1; @1 @2
inst_5829:

	clear_before(line_no);
	stack[0] = stack[0] * 11;

	snprintf(output, 200, "./chall $LINENO 3203 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 71 "skip_51"
inst_4895:

	clear_before(line_no);
	if (stack[0] == 71) {
	write_line("./chall $LINENO 1397", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 866", line_no + (w_line++));
	}
	leave(0);
// jne @2 95 "placebomb_n95"; @1 @2
inst_3662:

	clear_before(line_no);
	if (stack[1] != 95) {

	snprintf(output, 200, "./chall $LINENO 5539 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5159 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i39
inst_9531:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i39") ? getenv("i39") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 39 "skip_3"
inst_1470:

	clear_before(line_no);
	if (stack[0] == 39) {
	write_line("./chall $LINENO 2932", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 4866", line_no + (w_line++));
	}
	leave(0);
// add $rows 100000000 $rows
inst_3936:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3767", line_no + (w_line++));
	leave(0);
// jne @3 51 "getchar_n51"; @1 @2 @3
inst_9087:

	clear_before(line_no);
	if (stack[2] != 51) {

	snprintf(output, 200, "./chall $LINENO 9869 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 995 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 48 "skip_34"
inst_8045:

	clear_before(line_no);
	if (stack[0] == 48) {
	write_line("./chall $LINENO 2858", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 9056", line_no + (w_line++));
	}
	leave(0);
// add $rows 10000 $rows
inst_6322:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6849", line_no + (w_line++));
	leave(0);
// set $b3 38; @1
inst_8373:

	clear_before(line_no);
	tmp = 38;

	snprintf(output, 200, "export g_3049=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 7322 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 42 "placebomb_n42"; @1 @2
inst_5016:

	clear_before(line_no);
	if (stack[1] != 42) {

	snprintf(output, 200, "./chall $LINENO 6301 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1134 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 10 $squares
inst_5675:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5369", line_no + (w_line++));
	leave(0);
// add $b37 8 @1
inst_5145:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_4590") ? getenv("g_4590") : "0") + 8;

	snprintf(output, 200, "./chall $LINENO 3975 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b10 @6
inst_5211:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_5466") ? getenv("g_5466") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b15 66; @1
inst_2:

	clear_before(line_no);
	tmp = 66;

	snprintf(output, 200, "export g_1484=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 125 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 54 "getchar_n54"; @1 @2 @3
inst_8214:

	clear_before(line_no);
	if (stack[2] != 54) {

	snprintf(output, 200, "./chall $LINENO 8063 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5511 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// sub @5 8 @5; @1 @2 @3 @4 @5
inst_7226:

	clear_before(line_no);
	stack[4] = stack[4] - 8;

	snprintf(output, 200, "./chall $LINENO 972 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 100000000 $cols
inst_9824:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5082", line_no + (w_line++));
	leave(0);
// add $cols 10000 $cols
inst_9745:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3513", line_no + (w_line++));
	leave(0);
// set $b73 88
inst_9658:

	clear_before(line_no);
	tmp = 88;

	snprintf(output, 200, "export g_2189=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7065", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_5042:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 50 "skip_10"
inst_7612:

	clear_before(line_no);
	if (stack[0] == 50) {
	write_line("./chall $LINENO 5145", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 206", line_no + (w_line++));
	}
	leave(0);
// set $b76 67
inst_2697:

	clear_before(line_no);
	tmp = 67;

	snprintf(output, 200, "export g_4689=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9115", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b31 @6
inst_2902:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_9074") ? getenv("g_9074") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 100 $squares
inst_2349:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 830", line_no + (w_line++));
	leave(0);
// set $b38 66; @1
inst_4480:

	clear_before(line_no);
	tmp = 66;

	snprintf(output, 200, "export g_5505=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 5619 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jg @5 39 "getbomb_a40"; @1 @2 @3 @4 @5 @6
inst_3921:

	clear_before(line_no);
	if (stack[4] > 39) {

	snprintf(output, 200, "./chall $LINENO 6946 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8317 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i60
inst_2759:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i60") ? getenv("i60") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b83 @6
inst_5271:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_5081") ? getenv("g_5081") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b91 2 @1
inst_1209:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_6359") ? getenv("g_6359") : "0") + 2;

	snprintf(output, 200, "./chall $LINENO 3735 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 100000 $cols
inst_5039:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6996", line_no + (w_line++));
	leave(0);
// add $rows 100 $rows
inst_9424:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8134", line_no + (w_line++));
	leave(0);
// set $b30 63
inst_7338:

	clear_before(line_no);
	tmp = 63;

	snprintf(output, 200, "export g_3862=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2253", line_no + (w_line++));
	leave(0);
// mul @1 11 @1; @1 @2
inst_1926:

	clear_before(line_no);
	stack[0] = stack[0] * 11;

	snprintf(output, 200, "./chall $LINENO 1023 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 14 "placebomb_n14"; @1 @2
inst_7753:

	clear_before(line_no);
	if (stack[1] != 14) {

	snprintf(output, 200, "./chall $LINENO 9935 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4653 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b19 80; @1
inst_9161:

	clear_before(line_no);
	tmp = 80;

	snprintf(output, 200, "export g_3333=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 3699 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b12 39
inst_8885:

	clear_before(line_no);
	tmp = 39;

	snprintf(output, 200, "export g_9560=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1256", line_no + (w_line++));
	leave(0);
// set $b89 61
inst_1242:

	clear_before(line_no);
	tmp = 61;

	snprintf(output, 200, "export g_7572=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1482", line_no + (w_line++));
	leave(0);
// add $squares 10 $squares
inst_6257:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3246", line_no + (w_line++));
	leave(0);
// jne @5 52 "getbomb_n52"; @1 @2 @3 @4 @5 @6
inst_8759:

	clear_before(line_no);
	if (stack[4] != 52) {

	snprintf(output, 200, "./chall $LINENO 3581 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2110 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 32 "getbomb_n32"; @1 @2 @3 @4 @5 @6
inst_4550:

	clear_before(line_no);
	if (stack[4] != 32) {

	snprintf(output, 200, "./chall $LINENO 4054 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5966 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b88 @6
inst_8287:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_4690") ? getenv("g_4690") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_5665:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mod $b87 7 @1
inst_3521:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5115") ? getenv("g_5115") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO -1 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add @1 @2 $c2;
inst_8797:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_2431=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8922", line_no + (w_line++));
	leave(0);
// set $b28 31; @1
inst_2685:

	clear_before(line_no);
	tmp = 31;

	snprintf(output, 200, "export g_2399=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 5823 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// read
inst_2339:

	clear_before(line_no);
	write_line("read -n 100 input", line_no + (w_line++));
	write_line("export input", line_no + (w_line++));
	write_line("./chall $LINENO 8275", line_no + (w_line++));
	leave(0);
// set $b46 61
inst_9115:

	clear_before(line_no);
	tmp = 61;

	snprintf(output, 200, "export g_2206=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2373", line_no + (w_line++));
	leave(0);
// jne @3 0 "adj_bomb_1"; @1 @2 @3 @4 @5
inst_9623:

	clear_before(line_no);
	if (stack[2] != 0) {

	snprintf(output, 200, "./chall $LINENO 3849 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1533 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @3 18 "getchar_n18"; @1 @2 @3
inst_3096:

	clear_before(line_no);
	if (stack[2] != 18) {

	snprintf(output, 200, "./chall $LINENO 172 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6891 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 100000 $squares
inst_5410:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4059", line_no + (w_line++));
	leave(0);
// jmp "adj_bomb_start"; @1 @2 @3 @4 @2
inst_7186:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 9623 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 5 "getchar_n5"; @1 @2 @3
inst_2440:

	clear_before(line_no);
	if (stack[2] != 5) {

	snprintf(output, 200, "./chall $LINENO 9779 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1680 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// mod $b88 7 @1
inst_9690:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_4690") ? getenv("g_4690") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 6246 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b22 1 @1
inst_830:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_3650") ? getenv("g_3650") : "0") + 1;

	snprintf(output, 200, "./chall $LINENO 9689 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b24 31; @1
inst_7012:

	clear_before(line_no);
	tmp = 31;

	snprintf(output, 200, "export g_8262=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 3853 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 35 "getbomb_n35"; @1 @2 @3 @4 @5 @6
inst_4384:

	clear_before(line_no);
	if (stack[4] != 35) {

	snprintf(output, 200, "./chall $LINENO 3075 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2330 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i91
inst_7350:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i91") ? getenv("i91") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 105 "skip_7"
inst_4170:

	clear_before(line_no);
	if (stack[0] == 105) {
	write_line("./chall $LINENO 9305", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 7656", line_no + (w_line++));
	}
	leave(0);
// je @1 58 "skip_23"
inst_3477:

	clear_before(line_no);
	if (stack[0] == 58) {
	write_line("./chall $LINENO 3246", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 9004", line_no + (w_line++));
	}
	leave(0);
// jmp "getbomb"; @1 @2 @3 @4 @5 0
inst_2609:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4835 %d %d %d %d %d 0", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 66 "skip_20"
inst_2267:

	clear_before(line_no);
	if (stack[0] == 66) {
	write_line("./chall $LINENO 2195", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 2546", line_no + (w_line++));
	}
	leave(0);
// jne @3 20 "getchar_n20"; @1 @2 @3
inst_6002:

	clear_before(line_no);
	if (stack[2] != 20) {

	snprintf(output, 200, "./chall $LINENO 8245 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8932 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @2 81 "placebomb_n81"; @1 @2
inst_5217:

	clear_before(line_no);
	if (stack[1] != 81) {

	snprintf(output, 200, "./chall $LINENO 4357 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5466 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 30 "getbomb_n30"; @1 @2 @3 @4 @5 @6
inst_8317:

	clear_before(line_no);
	if (stack[4] != 30) {

	snprintf(output, 200, "./chall $LINENO 2683 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6100 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $rows 100 $rows
inst_8920:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1599", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_1656:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 46 "getchar_n46"; @1 @2 @3
inst_8494:

	clear_before(line_no);
	if (stack[2] != 46) {

	snprintf(output, 200, "./chall $LINENO 1247 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9048 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $rows 10 $rows
inst_768:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 424", line_no + (w_line++));
	leave(0);
// set $b49 3; @1
inst_8570:

	clear_before(line_no);
	tmp = 3;

	snprintf(output, 200, "export g_4387=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 2169 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b48 @6
inst_1394:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_8883") ? getenv("g_8883") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i18
inst_6891:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i18") ? getenv("i18") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 91 "getchar_n91"; @1 @2 @3
inst_4234:

	clear_before(line_no);
	if (stack[2] != 91) {

	snprintf(output, 200, "./chall $LINENO 4632 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7350 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 31 "getbomb_n31"; @1 @2 @3 @4 @5 @6
inst_2683:

	clear_before(line_no);
	if (stack[4] != 31) {

	snprintf(output, 200, "./chall $LINENO 4550 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2902 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "adj_bomb_calc"; @1 @2 @3 @4 @5
inst_4900:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 2609 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mod $b35 7 @1
inst_7911:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5050") ? getenv("g_5050") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 8172 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jg @2 19 "placebomb_a20"; @1 @2
inst_7762:

	clear_before(line_no);
	if (stack[1] > 19) {

	snprintf(output, 200, "./chall $LINENO 1768 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6041 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b21 50
inst_2438:

	clear_before(line_no);
	tmp = 50;

	snprintf(output, 200, "export g_5311=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2550", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i5
inst_1680:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i5") ? getenv("i5") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 1000000 $cols
inst_1502:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5312", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b92 @6
inst_2157:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_7693") ? getenv("g_7693") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b40 94; @1
inst_1767:

	clear_before(line_no);
	tmp = 94;

	snprintf(output, 200, "export g_8715=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 5207 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add @3 1 @3; @1 @2 @3 @4
inst_949:

	clear_before(line_no);
	stack[2] = stack[2] + 1;

	snprintf(output, 200, "./chall $LINENO 7186 %d %d %d %d", stack[0], stack[1], stack[2], stack[3]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_969:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mod $b92 7 @1
inst_3451:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_7693") ? getenv("g_7693") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 6533 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 85 "getchar_n85"; @1 @2 @3
inst_5530:

	clear_before(line_no);
	if (stack[2] != 85) {

	snprintf(output, 200, "./chall $LINENO 9838 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6624 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b72 @6
inst_4495:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_6199") ? getenv("g_6199") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i65
inst_1600:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i65") ? getenv("i65") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// nop
inst_8196:

	clear_before(line_no);
	write_line("./chall $LINENO 9173", line_no + (w_line++));
	leave(0);
// jne @5 74 "getbomb_n74"; @1 @2 @3 @4 @5 @6
inst_7363:

	clear_before(line_no);
	if (stack[4] != 74) {

	snprintf(output, 200, "./chall $LINENO 6582 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5795 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// mod $b86 7 @1
inst_9421:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_4895") ? getenv("g_4895") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 1631 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 57 "getbomb_n57"; @1 @2 @3 @4 @5 @6
inst_953:

	clear_before(line_no);
	if (stack[4] != 57) {

	snprintf(output, 200, "./chall $LINENO 2156 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 736 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @2 93 "placebomb_n93"; @1 @2
inst_6478:

	clear_before(line_no);
	if (stack[1] != 93) {

	snprintf(output, 200, "./chall $LINENO 8877 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4824 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 66 "skip_58"
inst_6601:

	clear_before(line_no);
	if (stack[0] == 66) {
	write_line("./chall $LINENO 4823", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 163", line_no + (w_line++));
	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b68 @6
inst_4849:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_1117") ? getenv("g_1117") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 1000000 $cols
inst_6420:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3594", line_no + (w_line++));
	leave(0);
// add @1 @2 $c3;
inst_6527:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_3694=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7911", line_no + (w_line++));
	leave(0);
// jne @3 7 "adj_finish"; @1 @2 @3 @4 @5
inst_1032:

	clear_before(line_no);
	if (stack[2] != 7) {

	snprintf(output, 200, "./chall $LINENO 2686 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 441 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 60 "skip_21"
inst_4105:

	clear_before(line_no);
	if (stack[0] == 60) {
	write_line("./chall $LINENO 2505", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 5776", line_no + (w_line++));
	}
	leave(0);
// add $cols 1000000 $cols
inst_2277:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1494", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b3 @6
inst_8737:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_3049") ? getenv("g_3049") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i64
inst_5918:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i64") ? getenv("i64") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i81
inst_6978:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i81") ? getenv("i81") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 5 "placebomb_n5"; @1 @2
inst_2539:

	clear_before(line_no);
	if (stack[1] != 5) {

	snprintf(output, 200, "./chall $LINENO 5499 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7868 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b58 10 @1
inst_4254:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_8511") ? getenv("g_8511") : "0") + 10;

	snprintf(output, 200, "./chall $LINENO 6601 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b87 87; @1
inst_1782:

	clear_before(line_no);
	tmp = 87;

	snprintf(output, 200, "export g_5115=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 5521 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 87 "getbomb_n87"; @1 @2 @3 @4 @5 @6
inst_3409:

	clear_before(line_no);
	if (stack[4] != 87) {

	snprintf(output, 200, "./chall $LINENO 7387 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7324 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $c3 27
inst_5544:

	clear_before(line_no);
	tmp = 27;

	snprintf(output, 200, "export g_3694=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1913", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_9454:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 51 "skip_22"
inst_9689:

	clear_before(line_no);
	if (stack[0] == 51) {
	write_line("./chall $LINENO 8408", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 871", line_no + (w_line++));
	}
	leave(0);
// add $rows 1000 $rows
inst_117:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5882", line_no + (w_line++));
	leave(0);
// add $cols 100 $cols
inst_7449:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4029", line_no + (w_line++));
	leave(0);
// jne @5 7 "getbomb_n7"; @1 @2 @3 @4 @5 @6
inst_9681:

	clear_before(line_no);
	if (stack[4] != 7) {

	snprintf(output, 200, "./chall $LINENO 4390 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8541 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b98 24; @1
inst_1612:

	clear_before(line_no);
	tmp = 24;

	snprintf(output, 200, "export g_2643=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 9454 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 75 "getchar_n75"; @1 @2 @3
inst_6750:

	clear_before(line_no);
	if (stack[2] != 75) {

	snprintf(output, 200, "./chall $LINENO 7573 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7320 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 95 "skip_54"
inst_4223:

	clear_before(line_no);
	if (stack[0] == 95) {
	write_line("./chall $LINENO 8400", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 8648", line_no + (w_line++));
	}
	leave(0);
// jne @3 63 "getchar_n63"; @1 @2 @3
inst_4162:

	clear_before(line_no);
	if (stack[2] != 63) {

	snprintf(output, 200, "./chall $LINENO 3559 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3040 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @3 84 "getchar_n84"; @1 @2 @3
inst_876:

	clear_before(line_no);
	if (stack[2] != 84) {

	snprintf(output, 200, "./chall $LINENO 5530 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1443 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $cols 10000 $cols
inst_1695:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4880", line_no + (w_line++));
	leave(0);
// jne @2 7 "placebomb_n7"; @1 @2
inst_1266:

	clear_before(line_no);
	if (stack[1] != 7) {

	snprintf(output, 200, "./chall $LINENO 3706 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 606 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b78 8 @1
inst_2592:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_7345") ? getenv("g_7345") : "0") + 8;

	snprintf(output, 200, "./chall $LINENO 9261 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 80 "skip_74"
inst_2557:

	clear_before(line_no);
	if (stack[0] == 80) {
	write_line("./chall $LINENO 4418", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 9861", line_no + (w_line++));
	}
	leave(0);
// jmp "placebomb_out"; @1
inst_3579:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 43 "getbomb_n43"; @1 @2 @3 @4 @5 @6
inst_8118:

	clear_before(line_no);
	if (stack[4] != 43) {

	snprintf(output, 200, "./chall $LINENO 6440 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 377 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 7 "adj_out7"; @1 @2
inst_4447:

	clear_before(line_no);
	if (stack[0] == 7) {

	snprintf(output, 200, "./chall $LINENO 1152 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6598 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b85 @6
inst_4360:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_8360") ? getenv("g_8360") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 55 "placebomb_n55"; @1 @2
inst_2075:

	clear_before(line_no);
	if (stack[1] != 55) {

	snprintf(output, 200, "./chall $LINENO 6431 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1207 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $cols 100000000 $cols
inst_424:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6024", line_no + (w_line++));
	leave(0);
// add $cols 1 $cols
inst_9032:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8860", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i77
inst_6255:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i77") ? getenv("i77") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 67 "getbomb_n67"; @1 @2 @3 @4 @5 @6
inst_4069:

	clear_before(line_no);
	if (stack[4] != 67) {

	snprintf(output, 200, "./chall $LINENO 8360 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8620 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b62 39
inst_2316:

	clear_before(line_no);
	tmp = 39;

	snprintf(output, 200, "export g_7985=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2027", line_no + (w_line++));
	leave(0);
// jne @2 87 "placebomb_n87"; @1 @2
inst_699:

	clear_before(line_no);
	if (stack[1] != 87) {

	snprintf(output, 200, "./chall $LINENO 2921 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1782 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b70 2 @1
inst_6426:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_7976") ? getenv("g_7976") : "0") + 2;

	snprintf(output, 200, "./chall $LINENO 7973 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add @1 @2 $c2;
inst_5247:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_2431=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 367", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b70 @6
inst_1272:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_7976") ? getenv("g_7976") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 10 "adj_out10"; @1 @2
inst_4070:

	clear_before(line_no);
	if (stack[0] == 10) {

	snprintf(output, 200, "./chall $LINENO 7770 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8974 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add @1 @2 $c3;
inst_7004:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_3694=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4736", line_no + (w_line++));
	leave(0);
// decrypt
inst_6917:

	clear_before(line_no);

	char* key = getenv("key");
	if (key == NULL) {
	  revert_file();
	  leave(0);
	}
	 char key_hex[32] = {};
	for (int i = 0; i < 64; i += 2) {
	  char byte_str[3] = {key[i], key[i + 1], '\0'};
	  key_hex[i / 2] = (char)strtol(byte_str, NULL, 16);
	}
	char decrypted[flag_len+2];
	for (int i = 0; i < flag_len; i++) {
	  decrypted[i] = flag[i] ^ key_hex[i % 32];
	}
	decrypted[flag_len+1] = '\0';
	decrypted[flag_len] = '\n';
	FILE *f = fopen("flag.txt", "w");
	if (f == NULL) {
	  printf("%s", decrypted);
	  revert_file();
	  leave(0);
	}
	fwrite(decrypted, sizeof(char), flag_len + 1, f);
	fclose(f);

	write_line("./chall $LINENO 5237", line_no + (w_line++));
	leave(0);
// add @1 @2 $c1
inst_6521:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_4968=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9343", line_no + (w_line++));
	leave(0);
// jne @3 24 "getchar_n24"; @1 @2 @3
inst_586:

	clear_before(line_no);
	if (stack[2] != 24) {

	snprintf(output, 200, "./chall $LINENO 7638 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8116 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "placebomb_out"; @1
inst_4299:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b99 1 @1
inst_8790:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_9888") ? getenv("g_9888") : "0") + 1;

	snprintf(output, 200, "./chall $LINENO 3451 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 12 "adj_out12"; @1 @2
inst_4438:

	clear_before(line_no);
	if (stack[0] == 12) {

	snprintf(output, 200, "./chall $LINENO 32 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9508 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 1000000 $squares
inst_6506:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4126", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i35
inst_8097:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i35") ? getenv("i35") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i30
inst_9582:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i30") ? getenv("i30") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add @1 @2 $c0
inst_7594:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_8694=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6306", line_no + (w_line++));
	leave(0);
// je @1 78 "skip_50"
inst_5588:

	clear_before(line_no);
	if (stack[0] == 78) {
	write_line("./chall $LINENO 3489", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 2620", line_no + (w_line++));
	}
	leave(0);
// je @1 98 "skip_18"
inst_4744:

	clear_before(line_no);
	if (stack[0] == 98) {
	write_line("./chall $LINENO 2570", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 8920", line_no + (w_line++));
	}
	leave(0);
// add $b10 7 @1
inst_8500:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5466") ? getenv("g_5466") : "0") + 7;

	snprintf(output, 200, "./chall $LINENO 7612 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 1000000 $cols
inst_147:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7248", line_no + (w_line++));
	leave(0);
// set $b6 65
inst_474:

	clear_before(line_no);
	tmp = 65;

	snprintf(output, 200, "export g_1827=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8211", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_131:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 44 "getbomb_n44"; @1 @2 @3 @4 @5 @6
inst_6440:

	clear_before(line_no);
	if (stack[4] != 44) {

	snprintf(output, 200, "./chall $LINENO 184 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9469 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $cols 100000000 $cols
inst_1657:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1269", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_1752:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 54 "getbomb_n54"; @1 @2 @3 @4 @5 @6
inst_202:

	clear_before(line_no);
	if (stack[4] != 54) {

	snprintf(output, 200, "./chall $LINENO 7139 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2584 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $rows 1000 $rows
inst_2890:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1556", line_no + (w_line++));
	leave(0);
// add $rows 10000000 $rows
inst_1214:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1329", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i90
inst_5764:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i90") ? getenv("i90") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_7322:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 10000 $squares
inst_9800:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6121", line_no + (w_line++));
	leave(0);
// jne @3 74 "getchar_n74"; @1 @2 @3
inst_6851:

	clear_before(line_no);
	if (stack[2] != 74) {

	snprintf(output, 200, "./chall $LINENO 6750 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6285 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// mod $b21 7 @1
inst_5386:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5311") ? getenv("g_5311") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 7855 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mul @1 11 @1; @1 @2
inst_1200:

	clear_before(line_no);
	stack[0] = stack[0] * 11;

	snprintf(output, 200, "./chall $LINENO 3196 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b52 8 @1
inst_8871:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_7774") ? getenv("g_7774") : "0") + 8;

	snprintf(output, 200, "./chall $LINENO 6707 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b79 39
inst_4523:

	clear_before(line_no);
	tmp = 39;

	snprintf(output, 200, "export g_7622=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4227", line_no + (w_line++));
	leave(0);
// add $squares 100000 $squares
inst_2585:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2858", line_no + (w_line++));
	leave(0);
// set $b85 45; @1
inst_4707:

	clear_before(line_no);
	tmp = 45;

	snprintf(output, 200, "export g_8360=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 7142 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b44 3 @1
inst_9387:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_1900") ? getenv("g_1900") : "0") + 3;

	snprintf(output, 200, "./chall $LINENO 5526 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b69 10; @1
inst_3381:

	clear_before(line_no);
	tmp = 10;

	snprintf(output, 200, "export g_8228=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 2039 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b55 9 @1
inst_7336:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_3935") ? getenv("g_3935") : "0") + 9;

	snprintf(output, 200, "./chall $LINENO 5181 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 10 $rows
inst_385:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 402", line_no + (w_line++));
	leave(0);
// jne @5 58 "getbomb_n58"; @1 @2 @3 @4 @5 @6
inst_2156:

	clear_before(line_no);
	if (stack[4] != 58) {

	snprintf(output, 200, "./chall $LINENO 38 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @3 13 "getchar_n13"; @1 @2 @3
inst_7637:

	clear_before(line_no);
	if (stack[2] != 13) {

	snprintf(output, 200, "./chall $LINENO 4769 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9463 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b11 1 @1
inst_4059:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_4545") ? getenv("g_4545") : "0") + 1;

	snprintf(output, 200, "./chall $LINENO 9567 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b53 @6
inst_5926:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_2671") ? getenv("g_2671") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_357:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add @5 1 @5; @1 @2 @3 @4 @5
inst_7546:

	clear_before(line_no);
	stack[4] = stack[4] + 1;

	snprintf(output, 200, "./chall $LINENO 2381 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mul $c2 13 @2; @1
inst_7168:

	clear_before(line_no);
	stack[1] = atoi(getenv("g_2431") ? getenv("g_2431") : "0") * 13;

	snprintf(output, 200, "./chall $LINENO 8797 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b4 @6
inst_4805:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_4910") ? getenv("g_4910") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 8 "getchar_n8"; @1 @2 @3
inst_6785:

	clear_before(line_no);
	if (stack[2] != 8) {

	snprintf(output, 200, "./chall $LINENO 7516 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1402 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b5 2 @1
inst_2858:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_2328") ? getenv("g_2328") : "0") + 2;

	snprintf(output, 200, "./chall $LINENO 9127 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 25 "getbomb_n25"; @1 @2 @3 @4 @5 @6
inst_8951:

	clear_before(line_no);
	if (stack[4] != 25) {

	snprintf(output, 200, "./chall $LINENO 2985 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5960 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @2 27 "placebomb_n27"; @1 @2
inst_8653:

	clear_before(line_no);
	if (stack[1] != 27) {

	snprintf(output, 200, "./chall $LINENO 3214 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4376 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 94 "skip_13"
inst_2496:

	clear_before(line_no);
	if (stack[0] == 94) {
	write_line("./chall $LINENO 5369", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 385", line_no + (w_line++));
	}
	leave(0);
// nop; $c0 @2
inst_4300:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6234 %d %d", atoi(getenv("g_8694") ? getenv("g_8694") : "0"), stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 95 "getchar_n95"; @1 @2 @3
inst_7309:

	clear_before(line_no);
	if (stack[2] != 95) {

	snprintf(output, 200, "./chall $LINENO 6625 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2216 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// exit
inst_4076:

	clear_before(line_no);
	revert_file();
	leave(0);
	leave(0);
// jmp "placebomb_out"; @1
inst_6793:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 1 $rows
inst_3904:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5743", line_no + (w_line++));
	leave(0);
// add $squares 1000 $squares
inst_9216:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1163", line_no + (w_line++));
	leave(0);
// add $cols 10000000 $cols
inst_5592:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6127", line_no + (w_line++));
	leave(0);
// jne @2 12 "placebomb_n12"; @1 @2
inst_6535:

	clear_before(line_no);
	if (stack[1] != 12) {

	snprintf(output, 200, "./chall $LINENO 6517 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5958 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $cols 100 $cols
inst_4982:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3406", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i79
inst_3254:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i79") ? getenv("i79") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 1 "adj_out1"; @1 @2
inst_9001:

	clear_before(line_no);
	if (stack[0] == 1) {

	snprintf(output, 200, "./chall $LINENO 3095 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 623 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b11 79
inst_5997:

	clear_before(line_no);
	tmp = 79;

	snprintf(output, 200, "export g_4545=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 436", line_no + (w_line++));
	leave(0);
// je @1 84 "skip_16"
inst_9038:

	clear_before(line_no);
	if (stack[0] == 84) {
	write_line("./chall $LINENO 830", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 9442", line_no + (w_line++));
	}
	leave(0);
// add $cols 10000000 $cols
inst_8108:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2585", line_no + (w_line++));
	leave(0);
// jmp "adj_bomb_calc"; @1 @2 @3 @4 @5
inst_2351:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 2609 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b85 7 @1
inst_4021:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_8360") ? getenv("g_8360") : "0") + 7;

	snprintf(output, 200, "./chall $LINENO 8631 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b8 @6
inst_2116:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_2788") ? getenv("g_2788") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 1000 $rows
inst_4038:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2737", line_no + (w_line++));
	leave(0);
// je @1 42 "skip_79"
inst_8133:

	clear_before(line_no);
	if (stack[0] == 42) {
	write_line("./chall $LINENO 6219", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 6115", line_no + (w_line++));
	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b60 @6
inst_2920:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_7980") ? getenv("g_7980") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 68 "placebomb_n68"; @1 @2
inst_6404:

	clear_before(line_no);
	if (stack[1] != 68) {

	snprintf(output, 200, "./chall $LINENO 3381 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3436 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// nop
inst_9173:

	clear_before(line_no);
	write_line("./chall $LINENO 6135", line_no + (w_line++));
	leave(0);
// set $b26 92
inst_9109:

	clear_before(line_no);
	tmp = 92;

	snprintf(output, 200, "export g_3812=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3541", line_no + (w_line++));
	leave(0);
// add $cols 1000000 $cols
inst_7306:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 223", line_no + (w_line++));
	leave(0);
// add $squares 100000 $squares
inst_9160:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1397", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_808:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 43 "placebomb_n43"; @1 @2
inst_6301:

	clear_before(line_no);
	if (stack[1] != 43) {

	snprintf(output, 200, "./chall $LINENO 5906 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9455 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "placebomb_out"; @1
inst_9802:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 1000000 $squares
inst_7108:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 203", line_no + (w_line++));
	leave(0);
// jmp "adj_bomb"; 9 50
inst_1798:

	clear_before(line_no);
	write_line("./chall $LINENO 7776 9 50", line_no + (w_line++));
	leave(0);
// mod @5 9 @6; @1 @2 @3 @4 @5
inst_5746:

	clear_before(line_no);
	stack[5] = stack[4] % 9;

	snprintf(output, 200, "./chall $LINENO 1364 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// echo "Enter the password:"
inst_1913:

	clear_before(line_no);
	write_line("echo \"Enter the password:\"", line_no + (w_line++));
	write_line("./chall $LINENO 2339", line_no + (w_line++));
	leave(0);
// add $rows 1 $rows
inst_4099:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5811", line_no + (w_line++));
	leave(0);
// jne @3 4 "adj_bomb_5"; @1 @2 @3 @4 @5
inst_3366:

	clear_before(line_no);
	if (stack[2] != 4) {

	snprintf(output, 200, "./chall $LINENO 8612 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6410 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// mul @1 11 @1; @1 @2
inst_2876:

	clear_before(line_no);
	stack[0] = stack[0] * 11;

	snprintf(output, 200, "./chall $LINENO 4733 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b51 @6
inst_7755:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_1277") ? getenv("g_1277") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b61 @6
inst_4995:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_8691") ? getenv("g_8691") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 107 "skip_78"
inst_9261:

	clear_before(line_no);
	if (stack[0] == 107) {
	write_line("./chall $LINENO 5128", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 1671", line_no + (w_line++));
	}
	leave(0);
// set $b82 87; @1
inst_8847:

	clear_before(line_no);
	tmp = 87;

	snprintf(output, 200, "export g_4711=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 254 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 10000 $squares
inst_691:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7577", line_no + (w_line++));
	leave(0);
// jne @3 4 "getchar_n4"; @1 @2 @3
inst_609:

	clear_before(line_no);
	if (stack[2] != 4) {

	snprintf(output, 200, "./chall $LINENO 2440 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8753 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b62 @6
inst_5143:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_7985") ? getenv("g_7985") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b39 @6
inst_979:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_9283") ? getenv("g_9283") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jg @6 7 "adj_bomb_inc"; @1 @2 @3 @4 @5
inst_1280:

	clear_before(line_no);
	if (stack[5] > 7) {

	snprintf(output, 200, "./chall $LINENO 949 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4092 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b78 99
inst_5778:

	clear_before(line_no);
	tmp = 99;

	snprintf(output, 200, "export g_7345=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 634", line_no + (w_line++));
	leave(0);
// set $b52 49
inst_5043:

	clear_before(line_no);
	tmp = 49;

	snprintf(output, 200, "export g_7774=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9510", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b97 @6
inst_5180:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_5572") ? getenv("g_5572") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jg @3 89 "getchar_a90"; @1 @2 @3
inst_4987:

	clear_before(line_no);
	if (stack[2] > 89) {

	snprintf(output, 200, "./chall $LINENO 258 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1337 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b67 8 @1
inst_8051:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_9076") ? getenv("g_9076") : "0") + 8;

	snprintf(output, 200, "./chall $LINENO 1943 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 44 "placebomb_n44"; @1 @2
inst_5906:

	clear_before(line_no);
	if (stack[1] != 44) {

	snprintf(output, 200, "./chall $LINENO 2946 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5881 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add @1 1 @1; @1 @2
inst_2475:

	clear_before(line_no);
	stack[0] = stack[0] + 1;

	snprintf(output, 200, "./chall $LINENO 4945 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 24 "placebomb_n24"; @1 @2
inst_8490:

	clear_before(line_no);
	if (stack[1] != 24) {

	snprintf(output, 200, "./chall $LINENO 1972 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7012 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 93 "getbomb_n93"; @1 @2 @3 @4 @5 @6
inst_5648:

	clear_before(line_no);
	if (stack[4] != 93) {

	snprintf(output, 200, "./chall $LINENO 9036 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6936 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @3 56 "getchar_n56"; @1 @2 @3
inst_6834:

	clear_before(line_no);
	if (stack[2] != 56) {

	snprintf(output, 200, "./chall $LINENO 9138 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5696 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 100000000 $squares
inst_5448:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2224", line_no + (w_line++));
	leave(0);
// add $rows 100000000 $rows
inst_6115:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3965", line_no + (w_line++));
	leave(0);
// jne @3 58 "getchar_n58"; @1 @2 @3
inst_8868:

	clear_before(line_no);
	if (stack[2] != 58) {

	snprintf(output, 200, "./chall $LINENO 3616 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5561 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "placebomb_out"; @1
inst_4686:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 92 "placebomb_n92"; @1 @2
inst_7862:

	clear_before(line_no);
	if (stack[1] != 92) {

	snprintf(output, 200, "./chall $LINENO 6478 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4364 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @2 98 "placebomb_n98"; @1 @2
inst_2070:

	clear_before(line_no);
	if (stack[1] != 98) {

	snprintf(output, 200, "./chall $LINENO 7996 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1612 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $b7 7 @1
inst_6296:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_8475") ? getenv("g_8475") : "0") + 7;

	snprintf(output, 200, "./chall $LINENO 4170 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 1 $cols
inst_4493:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 149", line_no + (w_line++));
	leave(0);
// set $b88 70
inst_9206:

	clear_before(line_no);
	tmp = 70;

	snprintf(output, 200, "export g_4690=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2491", line_no + (w_line++));
	leave(0);
// je @1 54 "skip_64"
inst_7511:

	clear_before(line_no);
	if (stack[0] == 54) {
	write_line("./chall $LINENO 544", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 177", line_no + (w_line++));
	}
	leave(0);
// add $rows 1000000 $rows
inst_163:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1435", line_no + (w_line++));
	leave(0);
// add $rows 100 $rows
inst_2546:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3836", line_no + (w_line++));
	leave(0);
// jne @2 41 "placebomb_n41"; @1 @2
inst_2777:

	clear_before(line_no);
	if (stack[1] != 41) {

	snprintf(output, 200, "./chall $LINENO 5016 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3984 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jg @5 99 "getbomb_out"; @1 @2 @3 @4 @5 @6
inst_7560:

	clear_before(line_no);
	if (stack[4] > 99) {

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3151 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @3 0 "getchar_n0"; @1 @2 @3
inst_3827:

	clear_before(line_no);
	if (stack[2] != 0) {

	snprintf(output, 200, "./chall $LINENO 4429 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6177 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "placebomb_out"; @1
inst_5500:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b35 8 @1
inst_6790:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5050") ? getenv("g_5050") : "0") + 8;

	snprintf(output, 200, "./chall $LINENO 6903 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// nop; $c0 @2
inst_9840:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 3068 %d %d", atoi(getenv("g_8694") ? getenv("g_8694") : "0"), stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 13 "getbomb_n13"; @1 @2 @3 @4 @5 @6
inst_6380:

	clear_before(line_no);
	if (stack[4] != 13) {

	snprintf(output, 200, "./chall $LINENO 7053 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1625 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jg @5 9 "getbomb_a10"; @1 @2 @3 @4 @5 @6
inst_4835:

	clear_before(line_no);
	if (stack[4] > 9) {

	snprintf(output, 200, "./chall $LINENO 5739 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8222 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $cols 1 $cols
inst_3383:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7123", line_no + (w_line++));
	leave(0);
// set $b30 10; @1
inst_6786:

	clear_before(line_no);
	tmp = 10;

	snprintf(output, 200, "export g_3862=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 8596 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 1000000 $rows
inst_8648:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3383", line_no + (w_line++));
	leave(0);
// jne @5 14 "getbomb_n14"; @1 @2 @3 @4 @5 @6
inst_7053:

	clear_before(line_no);
	if (stack[4] != 14) {

	snprintf(output, 200, "./chall $LINENO 3114 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6082 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b92 52; @1
inst_4364:

	clear_before(line_no);
	tmp = 52;

	snprintf(output, 200, "export g_7693=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 9136 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_7138:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b9 30
inst_4181:

	clear_before(line_no);
	tmp = 30;

	snprintf(output, 200, "export g_6560=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9899", line_no + (w_line++));
	leave(0);
// add $squares 100 $squares
inst_6127:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9418", line_no + (w_line++));
	leave(0);
// add @1 @2 $c1
inst_3196:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_4968=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8095", line_no + (w_line++));
	leave(0);
// add $b1 1 @1
inst_4320:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_8356") ? getenv("g_8356") : "0") + 1;

	snprintf(output, 200, "./chall $LINENO 3997 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 40 "getchar_n40"; @1 @2 @3
inst_3883:

	clear_before(line_no);
	if (stack[2] != 40) {

	snprintf(output, 200, "./chall $LINENO 622 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8572 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "placebomb_out"; @1
inst_885:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 100 $rows
inst_5776:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9089", line_no + (w_line++));
	leave(0);
// nop; @1 @4
inst_2686:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 3079 %d %d", stack[0], stack[3]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_1471:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b80 63
inst_4499:

	clear_before(line_no);
	tmp = 63;

	snprintf(output, 200, "export g_4706=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4181", line_no + (w_line++));
	leave(0);
// jne @2 4 "placebomb_n4"; @1 @2
inst_7580:

	clear_before(line_no);
	if (stack[1] != 4) {

	snprintf(output, 200, "./chall $LINENO 2539 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9448 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b51 17; @1
inst_3556:

	clear_before(line_no);
	tmp = 17;

	snprintf(output, 200, "export g_1277=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 2857 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 97 "skip_25"
inst_5270:

	clear_before(line_no);
	if (stack[0] == 97) {
	write_line("./chall $LINENO 9418", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 5498", line_no + (w_line++));
	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b16 @6
inst_6312:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_9230") ? getenv("g_9230") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 10 $cols
inst_5542:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8351", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_7703:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b81 @6
inst_9921:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_4635") ? getenv("g_4635") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b98 @6
inst_7848:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_2643") ? getenv("g_2643") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 70 "skip_55"
inst_5181:

	clear_before(line_no);
	if (stack[0] == 70) {
	write_line("./chall $LINENO 2091", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 9577", line_no + (w_line++));
	}
	leave(0);
// jg @3 29 "getchar_a30"; @1 @2 @3
inst_8250:

	clear_before(line_no);
	if (stack[2] > 29) {

	snprintf(output, 200, "./chall $LINENO 8832 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6002 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b46 3; @1
inst_640:

	clear_before(line_no);
	tmp = 3;

	snprintf(output, 200, "export g_2206=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 334 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b18 @6
inst_9688:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_8904") ? getenv("g_8904") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_2096:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 1 "adj_bomb_2"; @1 @2 @3 @4 @5
inst_3849:

	clear_before(line_no);
	if (stack[2] != 1) {

	snprintf(output, 200, "./chall $LINENO 2065 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5366 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add @1 @2 $c0
inst_3203:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_8694=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2586", line_no + (w_line++));
	leave(0);
// mul $c3 13 @2; @1
inst_7855:

	clear_before(line_no);
	stack[1] = atoi(getenv("g_3694") ? getenv("g_3694") : "0") * 13;

	snprintf(output, 200, "./chall $LINENO 4924 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b14 38; @1
inst_4653:

	clear_before(line_no);
	tmp = 38;

	snprintf(output, 200, "export g_2093=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 6031 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// sub @5 10 @5; @1 @2 @3 @4 @5
inst_9615:

	clear_before(line_no);
	stack[4] = stack[4] - 10;

	snprintf(output, 200, "./chall $LINENO 245 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_7142:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 100000 $rows
inst_3503:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4700", line_no + (w_line++));
	leave(0);
// add $squares 100000000 $squares
inst_223:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5128", line_no + (w_line++));
	leave(0);
// set $b73 17; @1
inst_5631:

	clear_before(line_no);
	tmp = 17;

	snprintf(output, 200, "export g_2189=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 4958 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mul @1 11 @1; @1 @2
inst_4627:

	clear_before(line_no);
	stack[0] = stack[0] * 11;

	snprintf(output, 200, "./chall $LINENO 5420 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 45 "getbomb_n45"; @1 @2 @3 @4 @5 @6
inst_184:

	clear_before(line_no);
	if (stack[4] != 45) {

	snprintf(output, 200, "./chall $LINENO 1743 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 875 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b11 87; @1
inst_7615:

	clear_before(line_no);
	tmp = 87;

	snprintf(output, 200, "export g_4545=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 566 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 46 "skip_12"
inst_3042:

	clear_before(line_no);
	if (stack[0] == 46) {
	write_line("./chall $LINENO 4254", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 8970", line_no + (w_line++));
	}
	leave(0);
// jne @5 47 "getbomb_n47"; @1 @2 @3 @4 @5 @6
inst_7147:

	clear_before(line_no);
	if (stack[4] != 47) {

	snprintf(output, 200, "./chall $LINENO 8852 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 572 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "placebomb_out"; @1
inst_6031:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i88
inst_8049:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i88") ? getenv("i88") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i66
inst_4613:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i66") ? getenv("i66") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 94 "getchar_n94"; @1 @2 @3
inst_5686:

	clear_before(line_no);
	if (stack[2] != 94) {

	snprintf(output, 200, "./chall $LINENO 7309 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6575 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $rows 10000 $rows
inst_7086:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2469", line_no + (w_line++));
	leave(0);
// add $rows 100000000 $rows
inst_2057:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4770", line_no + (w_line++));
	leave(0);
// add $cols 10 $cols
inst_1556:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7113", line_no + (w_line++));
	leave(0);
// mod $b11 7 @1
inst_8369:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_4545") ? getenv("g_4545") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 3193 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_566:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b39 87; @1
inst_1326:

	clear_before(line_no);
	tmp = 87;

	snprintf(output, 200, "export g_9283=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 4686 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b71 @6
inst_2447:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_6118") ? getenv("g_6118") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_9933:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_5636:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b50 @6
inst_8378:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_7833") ? getenv("g_7833") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i23
inst_9359:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i23") ? getenv("i23") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 26 "getchar_n26"; @1 @2 @3
inst_1391:

	clear_before(line_no);
	if (stack[2] != 26) {

	snprintf(output, 200, "./chall $LINENO 1314 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 383 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b35 @6
inst_2330:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_5050") ? getenv("g_5050") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mod $b98 7 @1
inst_6300:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_2643") ? getenv("g_2643") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 8225 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// print_flag
inst_5237:

	clear_before(line_no);
	write_line("echo 'cat flag.txt'", line_no + (w_line++));
	write_line("./chall $LINENO 2186", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i51
inst_995:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i51") ? getenv("i51") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i6
inst_434:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i6") ? getenv("i6") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 1000 $rows
inst_9056:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8108", line_no + (w_line++));
	leave(0);
// set $b38 96
inst_9899:

	clear_before(line_no);
	tmp = 96;

	snprintf(output, 200, "export g_5505=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8436", line_no + (w_line++));
	leave(0);
// je @1 48 "skip_40"
inst_2824:

	clear_before(line_no);
	if (stack[0] == 48) {
	write_line("./chall $LINENO 6121", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 6289", line_no + (w_line++));
	}
	leave(0);
// add $b2 1 @1
inst_5428:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_2812") ? getenv("g_2812") : "0") + 1;

	snprintf(output, 200, "./chall $LINENO 552 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b12 @6
inst_9826:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_9560") ? getenv("g_9560") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 98 "getchar_n98"; @1 @2 @3
inst_392:

	clear_before(line_no);
	if (stack[2] != 98) {

	snprintf(output, 200, "./chall $LINENO 2434 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1628 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "placebomb_out"; @1
inst_3614:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b93 @6
inst_6936:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_5063") ? getenv("g_5063") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i54
inst_5511:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i54") ? getenv("i54") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add @1 @2 $c1
inst_7192:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_4968=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8196", line_no + (w_line++));
	leave(0);
// add $squares 100000000 $squares
inst_3242:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6219", line_no + (w_line++));
	leave(0);
// mul @1 11 @1; @1 @2
inst_9915:

	clear_before(line_no);
	stack[0] = stack[0] * 11;

	snprintf(output, 200, "./chall $LINENO 7192 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 73 "placebomb_n73"; @1 @2
inst_8816:

	clear_before(line_no);
	if (stack[1] != 73) {

	snprintf(output, 200, "./chall $LINENO 2203 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5631 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $rows 10000000 $rows
inst_382:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7717", line_no + (w_line++));
	leave(0);
// jg @6 7 "adj_bomb_inc"; @1 @2 @3 @4 @5
inst_5609:

	clear_before(line_no);
	if (stack[5] > 7) {

	snprintf(output, 200, "./chall $LINENO 949 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3987 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 4 "getbomb_n4"; @1 @2 @3 @4 @5 @6
inst_836:

	clear_before(line_no);
	if (stack[4] != 4) {

	snprintf(output, 200, "./chall $LINENO 6355 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4805 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 46 "skip_63"
inst_2690:

	clear_before(line_no);
	if (stack[0] == 46) {
	write_line("./chall $LINENO 203", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 3448", line_no + (w_line++));
	}
	leave(0);
// set $b71 10; @1
inst_9163:

	clear_before(line_no);
	tmp = 10;

	snprintf(output, 200, "export g_6118=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 5636 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 100 $squares
inst_5082:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7513", line_no + (w_line++));
	leave(0);
// set $b59 48
inst_6756:

	clear_before(line_no);
	tmp = 48;

	snprintf(output, 200, "export g_3110=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8338", line_no + (w_line++));
	leave(0);
// add $cols 100 $cols
inst_8055:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6732", line_no + (w_line++));
	leave(0);
// add $squares 100000 $squares
inst_1269:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5164", line_no + (w_line++));
	leave(0);
// nop; $c1 @2
inst_7770:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 2876 %d %d", atoi(getenv("g_4968") ? getenv("g_4968") : "0"), stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_1286:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b21 10 @1
inst_1163:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5311") ? getenv("g_5311") : "0") + 10;

	snprintf(output, 200, "./chall $LINENO 4105 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 15 "getbomb_n15"; @1 @2 @3 @4 @5 @6
inst_3114:

	clear_before(line_no);
	if (stack[4] != 15) {

	snprintf(output, 200, "./chall $LINENO 8909 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1050 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i85
inst_6624:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i85") ? getenv("i85") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 23 "placebomb_n23"; @1 @2
inst_7737:

	clear_before(line_no);
	if (stack[1] != 23) {

	snprintf(output, 200, "./chall $LINENO 8490 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4443 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $rows 100 $rows
inst_9834:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1030", line_no + (w_line++));
	leave(0);
// add $b4 1 @1
inst_6697:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_4910") ? getenv("g_4910") : "0") + 1;

	snprintf(output, 200, "./chall $LINENO 6780 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 57 "placebomb_n57"; @1 @2
inst_7057:

	clear_before(line_no);
	if (stack[1] != 57) {

	snprintf(output, 200, "./chall $LINENO 4182 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6589 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 10000 $squares
inst_7332:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3302", line_no + (w_line++));
	leave(0);
// jmp "adj_bomb"; 10 19
inst_5136:

	clear_before(line_no);
	write_line("./chall $LINENO 7776 10 19", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b19 @6
inst_3091:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_3333") ? getenv("g_3333") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 94 "placebomb_n94"; @1 @2
inst_8877:

	clear_before(line_no);
	if (stack[1] != 94) {

	snprintf(output, 200, "./chall $LINENO 3662 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1479 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// sub @5 1 @5; @1 @2 @3 @4 @5
inst_517:

	clear_before(line_no);
	stack[4] = stack[4] - 1;

	snprintf(output, 200, "./chall $LINENO 2421 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mod $b57 7 @1
inst_9007:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_2988") ? getenv("g_2988") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 6055 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 82 "getchar_n82"; @1 @2 @3
inst_9707:

	clear_before(line_no);
	if (stack[2] != 82) {

	snprintf(output, 200, "./chall $LINENO 6357 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5066 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 48 "getbomb_n48"; @1 @2 @3 @4 @5 @6
inst_8852:

	clear_before(line_no);
	if (stack[4] != 48) {

	snprintf(output, 200, "./chall $LINENO 2566 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1394 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "placebomb_out"; @1
inst_6294:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 72 "getbomb_n72"; @1 @2 @3 @4 @5 @6
inst_7327:

	clear_before(line_no);
	if (stack[4] != 72) {

	snprintf(output, 200, "./chall $LINENO 4260 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4495 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "parseloop_start"; @1 @2
inst_4945:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6550 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 71 "skip_6"
inst_1161:

	clear_before(line_no);
	if (stack[0] == 71) {
	write_line("./chall $LINENO 8101", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 7180", line_no + (w_line++));
	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i36
inst_7064:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i36") ? getenv("i36") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 62 "skip_31"
inst_1044:

	clear_before(line_no);
	if (stack[0] == 62) {
	write_line("./chall $LINENO 1084", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 3678", line_no + (w_line++));
	}
	leave(0);
// jmp "placebomb_out"; @1
inst_2039:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b37 97
inst_9202:

	clear_before(line_no);
	tmp = 97;

	snprintf(output, 200, "export g_4590=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4454", line_no + (w_line++));
	leave(0);
// jne @5 36 "getbomb_n36"; @1 @2 @3 @4 @5 @6
inst_3075:

	clear_before(line_no);
	if (stack[4] != 36) {

	snprintf(output, 200, "./chall $LINENO 1033 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8023 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 1 $squares
inst_432:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2570", line_no + (w_line++));
	leave(0);
// add $b29 5 @1
inst_4126:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_9532") ? getenv("g_9532") : "0") + 5;

	snprintf(output, 200, "./chall $LINENO 5458 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 73 "getchar_n73"; @1 @2 @3
inst_9128:

	clear_before(line_no);
	if (stack[2] != 73) {

	snprintf(output, 200, "./chall $LINENO 6851 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 393 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 61 "skip_33"
inst_587:

	clear_before(line_no);
	if (stack[0] == 61) {
	write_line("./chall $LINENO 945", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 2273", line_no + (w_line++));
	}
	leave(0);
// set $b19 33
inst_3932:

	clear_before(line_no);
	tmp = 33;

	snprintf(output, 200, "export g_3333=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3711", line_no + (w_line++));
	leave(0);
// je @1 77 "skip_1"
inst_3997:

	clear_before(line_no);
	if (stack[0] == 77) {
	write_line("./chall $LINENO 8122", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 9023", line_no + (w_line++));
	}
	leave(0);
// add $rows 10 $rows
inst_6810:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9591", line_no + (w_line++));
	leave(0);
// add $b53 5 @1
inst_4706:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_2671") ? getenv("g_2671") : "0") + 5;

	snprintf(output, 200, "./chall $LINENO 8982 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 56 "getbomb_n56"; @1 @2 @3 @4 @5 @6
inst_860:

	clear_before(line_no);
	if (stack[4] != 56) {

	snprintf(output, 200, "./chall $LINENO 953 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3255 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jz @3 "parse_finish"; @1 @2 @3
inst_4766:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 706 %d && ./chall $LINENO 3839 %d %d %d || ./chall $LINENO 446 %d %d %d", stack[2], stack[0], stack[1], stack[2], stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b86 3 @1
inst_616:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_4895") ? getenv("g_4895") : "0") + 3;

	snprintf(output, 200, "./chall $LINENO 8663 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 10 "placebomb_n10"; @1 @2
inst_6041:

	clear_before(line_no);
	if (stack[1] != 10) {

	snprintf(output, 200, "./chall $LINENO 9599 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8319 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b86 45; @1
inst_195:

	clear_before(line_no);
	tmp = 45;

	snprintf(output, 200, "export g_4895=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 7138 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mod $b96 7 @1
inst_8663:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_7748") ? getenv("g_7748") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 4030 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 57 "getchar_n57"; @1 @2 @3
inst_9138:

	clear_before(line_no);
	if (stack[2] != 57) {

	snprintf(output, 200, "./chall $LINENO 8868 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9781 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @3 53 "getchar_n53"; @1 @2 @3
inst_1222:

	clear_before(line_no);
	if (stack[2] != 53) {

	snprintf(output, 200, "./chall $LINENO 8214 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3717 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "placebomb_out"; @1
inst_5096:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b6 6 @1
inst_3489:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_1827") ? getenv("g_1827") : "0") + 6;

	snprintf(output, 200, "./chall $LINENO 1161 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b68 17; @1
inst_3436:

	clear_before(line_no);
	tmp = 17;

	snprintf(output, 200, "export g_1117=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 969 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 46 "skip_57"
inst_5337:

	clear_before(line_no);
	if (stack[0] == 46) {
	write_line("./chall $LINENO 4693", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 5851", line_no + (w_line++));
	}
	leave(0);
// add $cols 1000 $cols
inst_3112:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9227", line_no + (w_line++));
	leave(0);
// set $b5 31; @1
inst_7868:

	clear_before(line_no);
	tmp = 31;

	snprintf(output, 200, "export g_2328=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 1286 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 1000000 $squares
inst_3406:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4418", line_no + (w_line++));
	leave(0);
// add $rows 10000000 $rows
inst_5612:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1695", line_no + (w_line++));
	leave(0);
// jne @5 26 "getbomb_n26"; @1 @2 @3 @4 @5 @6
inst_2985:

	clear_before(line_no);
	if (stack[4] != 26) {

	snprintf(output, 200, "./chall $LINENO 6781 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9140 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b78 45; @1
inst_1413:

	clear_before(line_no);
	tmp = 45;

	snprintf(output, 200, "export g_7345=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 885 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mul $c3 13 @2; @1
inst_6494:

	clear_before(line_no);
	stack[1] = atoi(getenv("g_3694") ? getenv("g_3694") : "0") * 13;

	snprintf(output, 200, "./chall $LINENO 6527 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 10 $squares
inst_1381:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6296", line_no + (w_line++));
	leave(0);
// jne @5 34 "getbomb_n34"; @1 @2 @3 @4 @5 @6
inst_3899:

	clear_before(line_no);
	if (stack[4] != 34) {

	snprintf(output, 200, "./chall $LINENO 4384 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3301 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 66 "getbomb_n66"; @1 @2 @3 @4 @5 @6
inst_8896:

	clear_before(line_no);
	if (stack[4] != 66) {

	snprintf(output, 200, "./chall $LINENO 4069 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8927 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 10000000 $squares
inst_5786:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5889", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_757:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b5 33
inst_1256:

	clear_before(line_no);
	tmp = 33;

	snprintf(output, 200, "export g_2328=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4523", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i56
inst_5696:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i56") ? getenv("i56") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b27 1 @1
inst_8101:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_8508") ? getenv("g_8508") : "0") + 1;

	snprintf(output, 200, "./chall $LINENO 2828 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b45 54
inst_152:

	clear_before(line_no);
	tmp = 54;

	snprintf(output, 200, "export g_5819=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3932", line_no + (w_line++));
	leave(0);
// set $b61 10; @1
inst_7220:

	clear_before(line_no);
	tmp = 10;

	snprintf(output, 200, "export g_8691=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 6999 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b84 58
inst_9734:

	clear_before(line_no);
	tmp = 58;

	snprintf(output, 200, "export g_3297=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8107", line_no + (w_line++));
	leave(0);
// echo "Incorrect!"
inst_9806:

	clear_before(line_no);
	write_line("echo \"Incorrect!\"", line_no + (w_line++));
	write_line("./chall $LINENO 4076", line_no + (w_line++));
	leave(0);
// jg @5 79 "getbomb_a80"; @1 @2 @3 @4 @5 @6
inst_6765:

	clear_before(line_no);
	if (stack[4] > 79) {

	snprintf(output, 200, "./chall $LINENO 5339 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2149 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 98 "getbomb_n98"; @1 @2 @3 @4 @5 @6
inst_8265:

	clear_before(line_no);
	if (stack[4] != 98) {

	snprintf(output, 200, "./chall $LINENO 9183 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7848 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @3 47 "getchar_n47"; @1 @2 @3
inst_1247:

	clear_before(line_no);
	if (stack[2] != 47) {

	snprintf(output, 200, "./chall $LINENO 8314 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6719 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// sub @3 49 @3; @1 @2
inst_9782:

	clear_before(line_no);
	stack[2] = stack[2] - 49;

	snprintf(output, 200, "./chall $LINENO 7415 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_6519:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_5823:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b84 @6
inst_4975:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_3297") ? getenv("g_3297") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 48 "getchar_n48"; @1 @2 @3
inst_8314:

	clear_before(line_no);
	if (stack[2] != 48) {

	snprintf(output, 200, "./chall $LINENO 3120 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4232 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 105 "skip_37"
inst_3975:

	clear_before(line_no);
	if (stack[0] == 105) {
	write_line("./chall $LINENO 1588", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 7086", line_no + (w_line++));
	}
	leave(0);
// jmp "placebomb_out"; @1
inst_4361:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 66 "getchar_n66"; @1 @2 @3
inst_450:

	clear_before(line_no);
	if (stack[2] != 66) {

	snprintf(output, 200, "./chall $LINENO 2167 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4613 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b58 56
inst_7065:

	clear_before(line_no);
	tmp = 56;

	snprintf(output, 200, "export g_8511=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2846", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_2735:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jl @6 1 "adj_bomb_inc"; @1 @2 @3 @4 @5
inst_281:

	clear_before(line_no);
	if (stack[5] < 1) {

	snprintf(output, 200, "./chall $LINENO 949 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6577 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 100 $squares
inst_6024:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 139", line_no + (w_line++));
	leave(0);
// jg @3 57 "parseloop_invalid"; @1 @2 @3
inst_75:

	clear_before(line_no);
	if (stack[2] > 57) {

	snprintf(output, 200, "./chall $LINENO 2016 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9782 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @3 67 "getchar_n67"; @1 @2 @3
inst_2167:

	clear_before(line_no);
	if (stack[2] != 67) {

	snprintf(output, 200, "./chall $LINENO 3518 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8843 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 40 "skip_27"
inst_2828:

	clear_before(line_no);
	if (stack[0] == 40) {
	write_line("./chall $LINENO 1163", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 4038", line_no + (w_line++));
	}
	leave(0);
// add $b45 3 @1
inst_745:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5819") ? getenv("g_5819") : "0") + 3;

	snprintf(output, 200, "./chall $LINENO 1372 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mod $b41 7 @1
inst_367:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_7941") ? getenv("g_7941") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 2638 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b17 5 @1
inst_8735:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_2426") ? getenv("g_2426") : "0") + 5;

	snprintf(output, 200, "./chall $LINENO 3046 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 1000000 $rows
inst_7347:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4521", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_6755:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 5 "adj_out5"; @1 @2
inst_9956:

	clear_before(line_no);
	if (stack[0] == 5) {

	snprintf(output, 200, "./chall $LINENO 647 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8240 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b95 @6
inst_8742:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_5601") ? getenv("g_5601") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b59 87; @1
inst_7228:

	clear_before(line_no);
	tmp = 87;

	snprintf(output, 200, "export g_3110=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 4218 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b62 3 @1
inst_6219:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_7985") ? getenv("g_7985") : "0") + 3;

	snprintf(output, 200, "./chall $LINENO 4591 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 37 "placebomb_n37"; @1 @2
inst_3701:

	clear_before(line_no);
	if (stack[1] != 37) {

	snprintf(output, 200, "./chall $LINENO 5314 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 7225 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b33 51
inst_3874:

	clear_before(line_no);
	tmp = 51;

	snprintf(output, 200, "export g_9342=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8024", line_no + (w_line++));
	leave(0);
// add $squares 1000 $squares
inst_6662:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6426", line_no + (w_line++));
	leave(0);
// add $rows 1000000 $rows
inst_8574:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 1000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2749", line_no + (w_line++));
	leave(0);
// set $b13 93
inst_4454:

	clear_before(line_no);
	tmp = 93;

	snprintf(output, 200, "export g_5551=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6353", line_no + (w_line++));
	leave(0);
// jne @2 48 "placebomb_n48"; @1 @2
inst_777:

	clear_before(line_no);
	if (stack[1] != 48) {

	snprintf(output, 200, "./chall $LINENO 8570 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8175 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// echo "Correct!"
inst_8234:

	clear_before(line_no);
	write_line("echo \"Correct!\"", line_no + (w_line++));
	write_line("./chall $LINENO 6917", line_no + (w_line++));
	leave(0);
// jne @5 38 "getbomb_n38"; @1 @2 @3 @4 @5 @6
inst_4764:

	clear_before(line_no);
	if (stack[4] != 38) {

	snprintf(output, 200, "./chall $LINENO 979 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4935 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b94 3; @1
inst_1479:

	clear_before(line_no);
	tmp = 3;

	snprintf(output, 200, "export g_8506=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 5065 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 97 "getchar_n97"; @1 @2 @3
inst_2951:

	clear_before(line_no);
	if (stack[2] != 97) {

	snprintf(output, 200, "./chall $LINENO 392 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 299 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 65 "getbomb_n65"; @1 @2 @3 @4 @5 @6
inst_9686:

	clear_before(line_no);
	if (stack[4] != 65) {

	snprintf(output, 200, "./chall $LINENO 8896 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 4502 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b44 @6
inst_9469:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_1900") ? getenv("g_1900") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 90 "placebomb_n90"; @1 @2
inst_5290:

	clear_before(line_no);
	if (stack[1] != 90) {

	snprintf(output, 200, "./chall $LINENO 7624 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8303 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i48
inst_4232:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i48") ? getenv("i48") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b81 74
inst_2243:

	clear_before(line_no);
	tmp = 74;

	snprintf(output, 200, "export g_4635=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9486", line_no + (w_line++));
	leave(0);
// jne @5 60 "getbomb_n60"; @1 @2 @3 @4 @5 @6
inst_6360:

	clear_before(line_no);
	if (stack[4] != 60) {

	snprintf(output, 200, "./chall $LINENO 295 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2920 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 87 "skip_67"
inst_1943:

	clear_before(line_no);
	if (stack[0] == 87) {
	write_line("./chall $LINENO 8197", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 5612", line_no + (w_line++));
	}
	leave(0);
// mul $c2 13 @2; @1
inst_6055:

	clear_before(line_no);
	stack[1] = atoi(getenv("g_2431") ? getenv("g_2431") : "0") * 13;

	snprintf(output, 200, "./chall $LINENO 5247 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mul @1 11 @1; @1 @2
inst_4846:

	clear_before(line_no);
	stack[0] = stack[0] * 11;

	snprintf(output, 200, "./chall $LINENO 7594 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b67 79
inst_3509:

	clear_before(line_no);
	tmp = 79;

	snprintf(output, 200, "export g_9076=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2697", line_no + (w_line++));
	leave(0);
// add $cols 1 $cols
inst_1599:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 432", line_no + (w_line++));
	leave(0);
// set $b56 97
inst_8939:

	clear_before(line_no);
	tmp = 97;

	snprintf(output, 200, "export g_6974=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7265", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i8
inst_1402:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i8") ? getenv("i8") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b98 8 @1
inst_9302:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_2643") ? getenv("g_2643") : "0") + 8;

	snprintf(output, 200, "./chall $LINENO 1209 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "adj_bomb"; 7 35
inst_6140:

	clear_before(line_no);
	write_line("./chall $LINENO 7776 7 35", line_no + (w_line++));
	leave(0);
// jmp "adj_bomb"; 6 56
inst_2586:

	clear_before(line_no);
	write_line("./chall $LINENO 7776 6 56", line_no + (w_line++));
	leave(0);
// jne @5 23 "getbomb_n23"; @1 @2 @3 @4 @5 @6
inst_8379:

	clear_before(line_no);
	if (stack[4] != 23) {

	snprintf(output, 200, "./chall $LINENO 7484 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2888 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "placebomb_out"; @1
inst_8481:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b59 @6
inst_38:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_3110") ? getenv("g_3110") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 10000 $cols
inst_9332:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1876", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i0
inst_6177:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i0") ? getenv("i0") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 65 "getchar_n65"; @1 @2 @3
inst_5051:

	clear_before(line_no);
	if (stack[2] != 65) {

	snprintf(output, 200, "./chall $LINENO 450 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1600 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @3 10 "getchar_n10"; @1 @2 @3
inst_4825:

	clear_before(line_no);
	if (stack[2] != 10) {

	snprintf(output, 200, "./chall $LINENO 7055 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6607 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jg @3 99 "getchar_out"; @1 @2 0
inst_258:

	clear_before(line_no);
	if (stack[2] > 99) {

	snprintf(output, 200, "./chall $LINENO 4766 %d %d 0", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9832 %d %d 0", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add $squares 100000000 $squares
inst_1725:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1025", line_no + (w_line++));
	leave(0);
// je @1 46 "skip_32"
inst_5891:

	clear_before(line_no);
	if (stack[0] == 46) {
	write_line("./chall $LINENO 7577", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 1299", line_no + (w_line++));
	}
	leave(0);
// jne @2 74 "placebomb_n74"; @1 @2
inst_2203:

	clear_before(line_no);
	if (stack[1] != 74) {

	snprintf(output, 200, "./chall $LINENO 9072 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9126 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b82 @6
inst_3721:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_4711") ? getenv("g_4711") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i1
inst_5328:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i1") ? getenv("i1") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i28
inst_2987:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i28") ? getenv("i28") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 100 $squares
inst_5312:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8101", line_no + (w_line++));
	leave(0);
// set $b91 96
inst_5235:

	clear_before(line_no);
	tmp = 96;

	snprintf(output, 200, "export g_6359=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6756", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b47 @6
inst_572:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_4136") ? getenv("g_4136") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 96 "getbomb_n96"; @1 @2 @3 @4 @5 @6
inst_7344:

	clear_before(line_no);
	if (stack[4] != 96) {

	snprintf(output, 200, "./chall $LINENO 5867 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1849 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// add @1 @2 $c2;
inst_7601:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_2431=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2337", line_no + (w_line++));
	leave(0);
// add $b43 3 @1
inst_4823:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_1611") ? getenv("g_1611") : "0") + 3;

	snprintf(output, 200, "./chall $LINENO 6192 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_4218:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mod $b24 7 @1
inst_4736:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_8262") ? getenv("g_8262") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 9928 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $rows 10000000 $rows
inst_8906:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 10000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 5039", line_no + (w_line++));
	leave(0);
// jne @5 76 "getbomb_n76"; @1 @2 @3 @4 @5 @6
inst_1312:

	clear_before(line_no);
	if (stack[4] != 76) {

	snprintf(output, 200, "./chall $LINENO 9242 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1048 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b0 3; @1
inst_982:

	clear_before(line_no);
	tmp = 3;

	snprintf(output, 200, "export g_3624=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 8645 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b13 10; @1
inst_9121:

	clear_before(line_no);
	tmp = 10;

	snprintf(output, 200, "export g_5551=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 3614 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 75 "skip_24"
inst_4457:

	clear_before(line_no);
	if (stack[0] == 75) {
	write_line("./chall $LINENO 8862", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 9834", line_no + (w_line++));
	}
	leave(0);
// set $b35 52; @1
inst_7565:

	clear_before(line_no);
	tmp = 52;

	snprintf(output, 200, "export g_5050=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 8128 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 100000 $cols
inst_9591:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1381", line_no + (w_line++));
	leave(0);
// add $rows 100000000 $rows
inst_3945:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6399", line_no + (w_line++));
	leave(0);
// add $cols 10000000 $cols
inst_6253:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 10000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2349", line_no + (w_line++));
	leave(0);
// set $b25 92
inst_436:

	clear_before(line_no);
	tmp = 92;

	snprintf(output, 200, "export g_9999=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1497", line_no + (w_line++));
	leave(0);
// sub @5 9 @5; @1 @2 @3 @4 @5
inst_6577:

	clear_before(line_no);
	stack[4] = stack[4] - 9;

	snprintf(output, 200, "./chall $LINENO 4900 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @5 41 "getbomb_n41"; @1 @2 @3 @4 @5 @6
inst_5760:

	clear_before(line_no);
	if (stack[4] != 41) {

	snprintf(output, 200, "./chall $LINENO 8221 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5493 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b62 59; @1
inst_1488:

	clear_before(line_no);
	tmp = 59;

	snprintf(output, 200, "export g_7985=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 2130 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b65 @6
inst_4502:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_5070") ? getenv("g_5070") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mod $b50 7 @1
inst_8922:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_7833") ? getenv("g_7833") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 4800 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b83 87; @1
inst_216:

	clear_before(line_no);
	tmp = 87;

	snprintf(output, 200, "export g_5081=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 4361 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b26 @6
inst_9140:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_3812") ? getenv("g_3812") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b39 93
inst_2524:

	clear_before(line_no);
	tmp = 93;

	snprintf(output, 200, "export g_9283=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6883", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b42 @6
inst_1324:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_1804") ? getenv("g_1804") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i31
inst_4813:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i31") ? getenv("i31") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b67 94; @1
inst_3620:

	clear_before(line_no);
	tmp = 94;

	snprintf(output, 200, "export g_9076=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 4031 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_2582:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// div @5 9 @6; @1 @2 @3 @4 @5
inst_1325:

	clear_before(line_no);
	stack[5] = stack[4] / 9;

	snprintf(output, 200, "./chall $LINENO 7872 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 100000000 $cols
inst_5430:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100000000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1679", line_no + (w_line++));
	leave(0);
// add $b95 2 @1
inst_4141:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_5601") ? getenv("g_5601") : "0") + 2;

	snprintf(output, 200, "./chall $LINENO 3521 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add @1 @2 $c0
inst_2527:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_8694=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1491", line_no + (w_line++));
	leave(0);
// jne @2 8 "placebomb_n8"; @1 @2
inst_3706:

	clear_before(line_no);
	if (stack[1] != 8) {

	snprintf(output, 200, "./chall $LINENO 4853 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8165 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// mul $c3 13 @2; @1
inst_7834:

	clear_before(line_no);
	stack[1] = atoi(getenv("g_3694") ? getenv("g_3694") : "0") * 13;

	snprintf(output, 200, "./chall $LINENO 1630 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mul $c3 13 @2; @1
inst_9928:

	clear_before(line_no);
	stack[1] = atoi(getenv("g_3694") ? getenv("g_3694") : "0") * 13;

	snprintf(output, 200, "./chall $LINENO 8610 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b35 57
inst_6353:

	clear_before(line_no);
	tmp = 57;

	snprintf(output, 200, "export g_5050=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 8939", line_no + (w_line++));
	leave(0);
// jne @3 61 "getchar_n61"; @1 @2 @3
inst_6761:

	clear_before(line_no);
	if (stack[2] != 61) {

	snprintf(output, 200, "./chall $LINENO 7721 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 2344 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i97
inst_299:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i97") ? getenv("i97") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "adj_bomb"; 5 47
inst_1491:

	clear_before(line_no);
	write_line("./chall $LINENO 7776 5 47", line_no + (w_line++));
	leave(0);
// jne @3 93 "getchar_n93"; @1 @2 @3
inst_4765:

	clear_before(line_no);
	if (stack[2] != 93) {

	snprintf(output, 200, "./chall $LINENO 5686 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 8505 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "placebomb_out"; @1
inst_125:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 100 $cols
inst_4991:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1512", line_no + (w_line++));
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b37 @6
inst_2878:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_4590") ? getenv("g_4590") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "adj_bomb"; 8 66
inst_8095:

	clear_before(line_no);
	write_line("./chall $LINENO 7776 8 66", line_no + (w_line++));
	leave(0);
// jmp "getchar_out"; @1 @2 $i82
inst_5066:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i82") ? getenv("i82") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 100 $squares
inst_584:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 100;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3554", line_no + (w_line++));
	leave(0);
// set $b51 67
inst_1497:

	clear_before(line_no);
	tmp = 67;

	snprintf(output, 200, "export g_1277=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 3864", line_no + (w_line++));
	leave(0);
// set $c0 93
inst_4726:

	clear_before(line_no);
	tmp = 93;

	snprintf(output, 200, "export g_8694=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1316", line_no + (w_line++));
	leave(0);
// jne @3 33 "getchar_n33"; @1 @2 @3
inst_8581:

	clear_before(line_no);
	if (stack[2] != 33) {

	snprintf(output, 200, "./chall $LINENO 3137 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3250 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b48 76
inst_5070:

	clear_before(line_no);
	tmp = 76;

	snprintf(output, 200, "export g_8883=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4944", line_no + (w_line++));
	leave(0);
// set $b97 98
inst_369:

	clear_before(line_no);
	tmp = 98;

	snprintf(output, 200, "export g_5572=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4284", line_no + (w_line++));
	leave(0);
// jne @2 53 "placebomb_n53"; @1 @2
inst_2983:

	clear_before(line_no);
	if (stack[1] != 53) {

	snprintf(output, 200, "./chall $LINENO 1036 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5486 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i22
inst_8659:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i22") ? getenv("i22") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i72
inst_6963:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i72") ? getenv("i72") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @2 36 "placebomb_n36"; @1 @2
inst_2925:

	clear_before(line_no);
	if (stack[1] != 36) {

	snprintf(output, 200, "./chall $LINENO 3701 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 6610 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b9 52; @1
inst_4853:

	clear_before(line_no);
	tmp = 52;

	snprintf(output, 200, "export g_6560=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 7703 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_334:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_1921:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "getchar_out"; @1 @2 $i13
inst_9463:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i13") ? getenv("i13") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b66 41
inst_5395:

	clear_before(line_no);
	tmp = 41;

	snprintf(output, 200, "export g_1559=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 9734", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_8286:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b29 66; @1
inst_6284:

	clear_before(line_no);
	tmp = 66;

	snprintf(output, 200, "export g_9532=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 1471 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 1000 $cols
inst_9089:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 1000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1522", line_no + (w_line++));
	leave(0);
// add $b56 5 @1
inst_203:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_6974") ? getenv("g_6974") : "0") + 5;

	snprintf(output, 200, "./chall $LINENO 893 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jne @3 78 "getchar_n78"; @1 @2 @3
inst_8931:

	clear_before(line_no);
	if (stack[2] != 78) {

	snprintf(output, 200, "./chall $LINENO 3254 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3437 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b2 33
inst_6585:

	clear_before(line_no);
	tmp = 33;

	snprintf(output, 200, "export g_2812=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7218", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_7663:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b55 80; @1
inst_1207:

	clear_before(line_no);
	tmp = 80;

	snprintf(output, 200, "export g_3935=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 6519 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $cols 100000 $cols
inst_7235:

	clear_before(line_no);
	tmp = atoi(getenv("g_1829") ? getenv("g_1829") : "0") + 100000;

	snprintf(output, 200, "export g_1829=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6257", line_no + (w_line++));
	leave(0);
// set $b96 30
inst_540:

	clear_before(line_no);
	tmp = 30;

	snprintf(output, 200, "export g_7748=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 2225", line_no + (w_line++));
	leave(0);
// add $rows 100000000 $rows
inst_1671:

	clear_before(line_no);
	tmp = atoi(getenv("g_7965") ? getenv("g_7965") : "0") + 100000000;

	snprintf(output, 200, "export g_7965=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7306", line_no + (w_line++));
	leave(0);
// je @1 65 "skip_46"
inst_900:

	clear_before(line_no);
	if (stack[0] == 65) {
	write_line("./chall $LINENO 8735", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 503", line_no + (w_line++));
	}
	leave(0);
// add @1 @2 $c1
inst_2124:

	clear_before(line_no);
	tmp = stack[0] + stack[1];

	snprintf(output, 200, "export g_4968=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 1798", line_no + (w_line++));
	leave(0);
// jne @5 46 "getbomb_n46"; @1 @2 @3 @4 @5 @6
inst_1743:

	clear_before(line_no);
	if (stack[4] != 46) {

	snprintf(output, 200, "./chall $LINENO 7147 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 9031 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @2 67 "placebomb_n67"; @1 @2
inst_4085:

	clear_before(line_no);
	if (stack[1] != 67) {

	snprintf(output, 200, "./chall $LINENO 6404 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3620 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jmp "getbomb_out"; @1 @2 @3 @4 $b30 @6
inst_6100:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 6775 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], atoi(getenv("g_3862") ? getenv("g_3862") : "0"), stack[5]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jmp "placebomb_out"; @1
inst_5321:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// add $squares 1000000 $squares
inst_8860:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 1000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 7409", line_no + (w_line++));
	leave(0);
// add $squares 10000000 $squares
inst_3805:

	clear_before(line_no);
	tmp = atoi(getenv("g_2184") ? getenv("g_2184") : "0") + 10000000;

	snprintf(output, 200, "export g_2184=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 4320", line_no + (w_line++));
	leave(0);
// add @3 81 @3; @1 @2 @3
inst_2429:

	clear_before(line_no);
	stack[2] = stack[2] + 81;

	snprintf(output, 200, "./chall $LINENO 9825 %d %d %d", stack[0], stack[1], stack[2]);
	write_line(output, line_no + (w_line++));

	leave(0);
// je @1 39 "skip_43"
inst_6192:

	clear_before(line_no);
	if (stack[0] == 39) {
	write_line("./chall $LINENO 5417", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 5794", line_no + (w_line++));
	}
	leave(0);
// jne @3 6 "adj_bomb_7"; @1 @2 @3 @4 @5
inst_988:

	clear_before(line_no);
	if (stack[2] != 6) {

	snprintf(output, 200, "./chall $LINENO 1032 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1325 %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 52 "skip_68"
inst_8499:

	clear_before(line_no);
	if (stack[0] == 52) {
	write_line("./chall $LINENO 9695", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 8906", line_no + (w_line++));
	}
	leave(0);
// jne @2 51 "placebomb_n51"; @1 @2
inst_3011:

	clear_before(line_no);
	if (stack[1] != 51) {

	snprintf(output, 200, "./chall $LINENO 619 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 3556 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 10 "getbomb_n10"; @1 @2 @3 @4 @5 @6
inst_8019:

	clear_before(line_no);
	if (stack[4] != 10) {

	snprintf(output, 200, "./chall $LINENO 6886 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5211 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// set $b61 95
inst_7937:

	clear_before(line_no);
	tmp = 95;

	snprintf(output, 200, "export g_8691=%d", tmp);
	write_line(output, line_no + (w_line++));

	write_line("./chall $LINENO 6585", line_no + (w_line++));
	leave(0);
// jmp "placebomb_out"; @1
inst_4628:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4432 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// jg @2 99 "placebomb_out"; @1 @2
inst_6445:

	clear_before(line_no);
	if (stack[1] > 99) {

	snprintf(output, 200, "./chall $LINENO 4432 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 5290 %d %d", stack[0], stack[1]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// jne @5 42 "getbomb_n42"; @1 @2 @3 @4 @5 @6
inst_8221:

	clear_before(line_no);
	if (stack[4] != 42) {

	snprintf(output, 200, "./chall $LINENO 8118 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}else{

	snprintf(output, 200, "./chall $LINENO 1324 %d %d %d %d %d %d", stack[0], stack[1], stack[2], stack[3], stack[4], stack[5]);
	write_line(output, line_no + (w_line++));

	}
	leave(0);
// je @1 56 "skip_17"
inst_3046:

	clear_before(line_no);
	if (stack[0] == 56) {
	write_line("./chall $LINENO 139", line_no + (w_line++));
	}else{
	write_line("./chall $LINENO 768", line_no + (w_line++));
	}
	leave(0);
// jmp "getchar_out"; @1 @2 $i45
inst_6970:

	clear_before(line_no);

	snprintf(output, 200, "./chall $LINENO 4766 %d %d %d", stack[0], stack[1], atoi(getenv("i45") ? getenv("i45") : "0"));
	write_line(output, line_no + (w_line++));

	leave(0);
// add $b73 5 @1
inst_1397:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_2189") ? getenv("g_2189") : "0") + 5;

	snprintf(output, 200, "./chall $LINENO 4245 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// set $b50 17; @1
inst_7043:

	clear_before(line_no);
	tmp = 17;

	snprintf(output, 200, "export g_7833=%d", tmp);
	write_line(output, line_no + (w_line++));


	snprintf(output, 200, "./chall $LINENO 7663 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
// mod $b90 7 @1
inst_3735:

	clear_before(line_no);
	stack[0] = atoi(getenv("g_4158") ? getenv("g_4158") : "0") % 7;

	snprintf(output, 200, "./chall $LINENO 6300 %d", stack[0]);
	write_line(output, line_no + (w_line++));

	leave(0);
}