require_relative "helper"

class TestElementPairing < TestCase
  def setup
    @pairing = Pairing.init_from_param_str(TYPE_A_PARAM)
    @e1 = Element.new(@pairing, ElementType_G1)
    @e2 = Element.new(@pairing, ElementType_G2)
    @z = Element.new(@pairing, ElementType_Zr)
    @e1.random
    @e2.random
    @z.random
  end

  def test_e
    assert_raise RuntimeError do
      e(Element.new, Element.new)
    end
    et = e(@e1, @e2)
    assert_equal ElementType_GT, et.type
    assert_equal et ** @z, e(@e1 ** @z, @e2)
  end

  def test_e_prod
    assert_raise ArgumentError do
      e_prod [], []
    end
    assert_raise ArgumentError do
      e_prod [@e1, @e1], [@e2]
    end
    assert_raise RuntimeError do
      e_prod([@e1], [Element.new])
    end
    et1 = e_prod [@e1, @e1], [@e2, @e2]
    et2 = e(@e1, @e2)
    assert_equal et2 * et2, et1
  end

  def test_pairing_fixed_g1
    assert_raise RuntimeError do
      FixedG1Pairing.new Element.new
    end
    assert_raise ArgumentError do
      FixedG1Pairing.new @zr
    end
    p = FixedG1Pairing.new @e1
    assert_equal @e1, p.get_g1
    assert_raise RuntimeError do
      p.apply Element.new
    end
    assert_equal e(@e1, @e2), p.apply(@e2)
  end
end
