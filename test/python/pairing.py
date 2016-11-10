from helper import *

class TestPairing(unittest.TestCase):
    def setUp(self):
        self.param = PairingParam.init_from_str(TYPE_A_PARAM)

    def test_init(self):
        with self.assertRaises(RuntimeError):
            Pairing.init_from_param_str("type a")
        pairing1 = Pairing.init_from_param(self.param)
        pairing2 = Pairing.init_from_param_str(self.param.to_str())
        self.assertEqual(pairing1.symmetric(), pairing2.symmetric())
        self.assertEqual(pairing1.g1_bytes_length(), pairing2.g1_bytes_length())
        self.assertEqual(pairing1.g2_bytes_length(), pairing2.g2_bytes_length())

    def test_info(self):
        pairing = Pairing.init_from_param(self.param)
        self.assertTrue(pairing.symmetric())
        self.assertEqual(pairing.g1_bytes_length(), 128)
        self.assertEqual(pairing.g1_x_only_bytes_length(), 64)
        self.assertEqual(pairing.g1_compressed_bytes_length(), 65)
        self.assertEqual(pairing.g2_bytes_length(), 128)
        self.assertEqual(pairing.g2_x_only_bytes_length(), 64)
        self.assertEqual(pairing.g2_compressed_bytes_length(), 65)
        self.assertEqual(pairing.gt_bytes_length(), 128)
        self.assertEqual(pairing.zr_bytes_length(), 20)

if __name__ == '__main__':
    unittest.main()
