#ifndef _GXCO_H_
#define _GXCO_H_

#include <stdint.h>
#include <vector>
#include <string>
#include <initializer_list>
#include <ostream>
#include <iostream>
#include <sstream>
#include <memory>
#include <unordered_map>
#include <assert.h>
#include <functional>
#include <array>

#ifndef GXCO_DIRECT_EMIT
#define GXCO_DIRECT_EMIT 0
#endif

namespace gxco {

enum arch_t
{
    GFX906  = 906,
    GFX908  = 908,
    GFX90A  = 910,
    GFX1030 = 1030,
    GFX1100 = 1100,
};

#define MODIFIER_NA "na"
// operand_t modifier
#define MODIFIER_ABS "abx"
#define MODIFIER_NEG "neg"

// instruction_t modifier
// LDS, global,
#define MODIFIER_OFFSET "offset" // if we have 2 content, emit offset0/offset1

#define MODIFIER_GDS "gds"
#define MODIFIER_DLC "dlc"
#define MODIFIER_GLC "glc"
#define MODIFIER_LDS "lds"
#define MODIFIER_SLC "slc"
#define MODIFIER_TFE "tfe"
#define MODIFIER_NV "nv"
#define MODIFIER_SC0 "sc0"
#define MODIFIER_SC1 "sc1"
#define MODIFIER_NT "nt"

#define MODIFIER_IDXEN "idxen"
#define MODIFIER_OFFEN "offen"

// DPP8
#define MODIFIER_DPP8 "dpp8"
#define MODIFIER_FI "fi"

// DPP
#define MODIFIER_QUAD_PERM "quad_perm"

// SDWA
#define MODIFIER_DST_SEL "dst_sel"
#define MODIFIER_DST_UNUSED "dst_unused"
#define MODIFIER_SRC0_SEL "src0_sel"
#define MODIFIER_SRC1_SEL "src1_sel"

// OP_SEL
#define MODIFIER_SEL_LO "sel_lo"
#define MODIFIER_SEL_HI "sel_hi"

#define MODIFIER_CLAMP "clamp"
#define MODIFIER_OMOD_MUL_2 "mul:2"
#define MODIFIER_OMOD_MUL_4 "mul:4"
#define MODIFIER_OMOD_DIV_2 "div:2"

// mfma
#define MODIFIER_CBSZ "cbsz"
#define MODIFIER_ABID "abid"
#define MODIFIER_BLGP "blgp"

enum modifier_enum_symbol
{
    MODIFIER_SYMBOL_DWORD = 0,
    MODIFIER_SYMBOL_BYTE0,
    MODIFIER_SYMBOL_BYTE1,
    MODIFIER_SYMBOL_BYTE2,
    MODIFIER_SYMBOL_BYTE3,

    MODIFIER_SYMBOL_WORD0,
    MODIFIER_SYMBOL_WORD1,

    MODIFIER_SYMBOL_UNUSED_PAD,
    MODIFIER_SYMBOL_UNUSED_SEXT,
    MODIFIER_SYMBOL_UNUSED_PRESERVE,
};

enum operand_type
{
    OPERAND_UNUSED = 0,
    OPERAND_REGISTER_SGPR,
    OPERAND_REGISTER_VGPR,
    OPERAND_REGISTER_AGPR,
    OPERAND_REGISTER_HWREG,
    OPERAND_WAITCNT,
    OPERAND_VARIABLE, // symbol can be a register, or const value
    OPERAND_IMMEDIATE,
    OPERAND_LABEL,
    OPERAND_MACRO_ARG,
};

enum operand_flags
{
    BITS_ABS,
};

enum encoding
{
    ENC_VOP1 = 0,
    ENC_VOP2,
    ENC_VOP3,
    ENC_VOP3P,
    ENC_VOP3C,

    ENC_SOP1,
    ENC_SOP2,
    ENC_SOPC,
    ENC_SOPK,
    ENC_SOPP,

    ENC_SMEM,
    ENC_MUBUF,
    ENC_DS,
    ENC_FLAT,

    ENC_FAKE_COMMENT_LINE,
    ENC_FAKE_NEW_LINE,
    ENC_FAKE_LABEL,

    ENC_MACRO,
};

enum emitter_type
{
    EMITTER_LLVM_HSA,
    EMITTER_SP3,
};

enum hsa_code_object_version
{
    HSA_CODE_OBJECT_V3,
    HSA_CODE_OBJECT_V4
};

class target_info_t
{
public:
    arch_t arch;
};

// register allocator, linearly grow
class ra_linear_t
{
public:
    ra_linear_t() : cnt(0) {}
    ra_linear_t(uint32_t cnt_) : cnt(cnt_) {}
    uint32_t operator()(uint32_t num, uint32_t alignment = 0)
    {
        if(alignment != 0)
        {
            uint32_t aligned_cnt = ((cnt + alignment - 1) / alignment) * alignment;
            cnt                  = aligned_cnt;
        }
        uint32_t previous_cnt = cnt;
        cnt += num;
        return previous_cnt;
    }
    uint32_t operator()() { return cnt; }
    uint32_t get(uint32_t alignment = 0) const
    {
        if(alignment == 0)
            return cnt;
        return ((cnt + alignment - 1) / alignment) * alignment;
    }

private:
    uint32_t cnt;
};

template <operand_type register_type>
class indexed_register_t;
using vgpr_t = indexed_register_t<OPERAND_REGISTER_VGPR>;
using sgpr_t = indexed_register_t<OPERAND_REGISTER_SGPR>;
using agpr_t = indexed_register_t<OPERAND_REGISTER_AGPR>;
class variable_t;
class macro_args_t;
// class imm_t;
class hwreg_t;

// modifier can be applied to instruction_t (like dpp8, bound_ctrl...)
// or applied to operand_t (like abs, neg... )
class modifier_t
{
public:
    modifier_t() : name(MODIFIER_NA) {}
    modifier_t(std::string name_) : name(name_) {}
    modifier_t(std::string name_, int32_t value_) : name(name_), value{value_} {}
    modifier_t(std::string name_, std::initializer_list<int32_t> value_)
        : name(name_), value(value_)
    {
    }
    modifier_t(std::string name_, std::string value_str_) : name(name_), value_str{value_str_} {}

    virtual std::unique_ptr<modifier_t> clone() const
    {
        return std::make_unique<modifier_t>(*this);
    }

    std::string get_name() const { return name; }

    void expr_to_instruction(std::ostream& os_) const
    {
        if(name == "offset" && value.size() == 2)
        {
            // special case for offset. if 2 values, need emit offset0:xx offset1:xx
            os_ << "offset0:" << value[0] << " offset1:" << value[1];
            return;
        }
        os_ << name;

        if(value.empty() && !value_str.empty())
        {
            os_ << ":" << value_str;
        }
        else
        {
            if(value.size() == 1)
            {
                os_ << ":" << value[0];
            }
            else if(value.size() > 1)
            {
                os_ << ":[" << value[0];
                for(uint32_t i = 1; i < value.size(); i++)
                {
                    os_ << "," << value[i];
                }
                os_ << "]";
            }
        }
    }
    void expr(std::ostream& os_) const { expr_to_instruction(os_); }

private:
    std::string name;
    std::vector<int32_t> value; // some modifier have value
    std::string value_str;      // some modifier have string
};

class operand_t
{
public:
    operand_t() : type(OPERAND_UNUSED) {}
    explicit operand_t(operand_type t_) : type(t_) {}
    operand_t(int32_t value_) : type(OPERAND_IMMEDIATE), imm_buf(std::to_string(value_)) {}
    operand_t(uint32_t value_) : type(OPERAND_IMMEDIATE), imm_buf(std::to_string(value_)) {}
    operand_t(float value_) : type(OPERAND_IMMEDIATE), imm_buf(std::to_string(value_)) {}
    virtual ~operand_t() {}

    virtual std::unique_ptr<operand_t> clone() const { return std::make_unique<operand_t>(*this); }

    operand_type get_type() const { return type; }

    const macro_args_t& as_macro_arg() const;
    const variable_t& as_variable() const;
    const vgpr_t& as_vgpr() const;
    const sgpr_t& as_sgpr() const;
    const agpr_t& as_agpr() const;
    const hwreg_t& as_hwreg() const;

    virtual void expr(std::ostream& os_) const
    {
        // std::cout<< " expr operand:"<< type<<std::endl;
        if(type == OPERAND_IMMEDIATE)
            os_ << imm_buf;
    }

    operand_t& modify(const modifier_t& mod_)
    {
        modifier.push_back(mod_);
        return *this;
    }
    const std::vector<modifier_t>& get_modifier() const { return modifier; }

    std::string get_imm() const { return imm_buf; }

protected:
    operand_type type;
    std::vector<modifier_t> modifier; // the first item is the inner most opf
    std::string imm_buf;
};

// TODO: this takes care of all immediate value smaller than 32bit
// class imm_t : public operand_t {
// public:
//    imm_t(int32_t value_) : operand_t(OPERAND_IMMEDIATE), imm_buf(std::to_string(value_)) {}
//    imm_t(float value_) : operand_t(OPERAND_IMMEDIATE), imm_buf(std::to_string(value_)) {}
//    virtual ~imm_t() {}
//    virtual std::unique_ptr<operand_t> clone() const { return std::make_unique<imm_t>(*this); }
//    std::string get_imm() const {return imm_buf; }
// private:
//    std::string imm_buf;
//};

template <operand_type register_type>
class indexed_register_t : public operand_t
{
public:
    indexed_register_t(uint32_t i_start_, uint32_t i_end_)
        : operand_t(register_type), indices{i_start_, i_end_}
    {
    }
    indexed_register_t(uint32_t i_) : indexed_register_t(i_, i_) {}
    virtual ~indexed_register_t() {}
    virtual std::unique_ptr<operand_t> clone() const
    {
        return std::make_unique<indexed_register_t>(*this);
    }

    // create a new instance by offset. note here only indices[0] will be used as start offset.
    virtual indexed_register_t operator()(uint32_t offset_) const
    {
        return indexed_register_t(indices[0] + offset_);
    }
    virtual indexed_register_t operator()(uint32_t offset_start_, uint32_t offset_end_) const
    {
        return indexed_register_t(indices[0] + offset_start_, indices[0] + offset_end_);
    }

    static std::string get_prefix(operand_type type_)
    {
        switch(type_)
        {
        case OPERAND_REGISTER_VGPR: return "v"; break;
        case OPERAND_REGISTER_SGPR: return "s"; break;
        case OPERAND_REGISTER_AGPR: return "a"; break;
        default: return ""; break;
        }
    }

    virtual void expr(std::ostream& os_) const
    {
        std::string prefix = get_prefix(type);
        if(is_range_index())
            os_ << prefix << "[" << indices[0] << ":" << indices[1] << "]";
        else
            os_ << prefix << "[" << indices[0] << "]";
    }

    bool is_range_index() const { return indices[0] < indices[1]; }
    uint32_t get_index(uint32_t id_) const
    {
        assert(id_ == 0 || id_ == 1);
        return indices[id_];
    }

private:
    uint32_t indices[2];
};

// every counter have 8 bit. if the value is 0xff, indicate not turn on this counter
// in real hw use a single simm16 to encode
#define VMCNT_GET(x) (((x) >> 0) & 0xff)
#define LGKMCNT_GET(x) (((x) >> 8) & 0xff)
#define EXPCNT_GET(x) (((x) >> 16) & 0xff)
#define VSCNT_GET(x) (((x) >> 24) & 0xff)

#define VMCNT(x) (((uint32_t)(x)&0xff) << 0)
#define LGKMCNT(x) (((uint32_t)(x)&0xff) << 8)
#define EXPCNT(x) (((uint32_t)(x)&0xff) << 16)
#define VSCNT(x) (((uint32_t)(x)&0xff) << 24)

class waitcnt_t : public operand_t
{
public:
    waitcnt_t(uint32_t cnt_) : operand_t(OPERAND_WAITCNT), cnt(cnt_) {}
    virtual std::unique_ptr<operand_t> clone() const { return std::make_unique<waitcnt_t>(*this); }

    virtual void expr(std::ostream& os_) const
    {
        std::string delim  = "";
        uint32_t c_vmcnt   = VMCNT_GET(cnt);
        uint32_t c_lgkmcnt = LGKMCNT_GET(cnt);
        uint32_t c_expcnt  = EXPCNT_GET(cnt);
        uint32_t c_vscnt   = VSCNT_GET(cnt);
        if(c_vmcnt != 0xff)
        {
            os_ << delim << "vmcnt(" << c_vmcnt << ")";
            delim = " & ";
        }
        if(c_lgkmcnt != 0xff)
        {
            os_ << delim << "lgkmcnt(" << c_lgkmcnt << ")";
            delim = " & ";
        }
        if(c_expcnt != 0xff)
        {
            os_ << delim << "expcnt(" << c_expcnt << ")";
            delim = " & ";
        }
        if(c_vscnt != 0xff)
        {
            os_ << delim << "vscnt(" << c_vscnt << ")";
            // delim = " & ";
        }
    }

private:
    uint32_t cnt;
};

// vcc, exec....
class hwreg_t : public operand_t
{
public:
    hwreg_t(std::string name_) : operand_t(OPERAND_REGISTER_HWREG), name(name_) {}
    virtual std::unique_ptr<operand_t> clone() const { return std::make_unique<hwreg_t>(*this); }
    std::string get_name() const { return name; }

    virtual void expr(std::ostream& os_) const { os_ << name; }

private:
    std::string name;
};

class label_t : public operand_t
{
public:
    label_t(std::string label_) : operand_t(OPERAND_LABEL), label(label_) {}
    virtual std::unique_ptr<operand_t> clone() const { return std::make_unique<label_t>(*this); }
    std::string get_label() const { return label; }

    virtual void expr(std::ostream& os_) const { os_ << label; }

private:
    std::string label;
};

// we have a spacial rule here
// if the variable start with s_/v_/a_/k_, it indicate a variable reference to a
// sgpr/vgpr/agpr/immediate_value hwreg, no need to reference
class variable_t : public operand_t
{
public:
    variable_t(std::string name_, std::string value_, uint32_t i_start_, uint32_t i_end_)
        : operand_t(OPERAND_VARIABLE), name(name_), value(value_), indices{i_start_, i_end_}
    {
    }
    variable_t(std::string name_, std::string value_, uint32_t i_start_)
        : variable_t(name_, value_, i_start_, i_start_)
    {
    }
    variable_t(std::string name_, std::string value_) : variable_t(name_, value_, 0, 0) {}
    variable_t(std::string name_, uint32_t value_) : variable_t(name_, std::to_string(value_), 0, 0)
    {
    }
    variable_t(std::string name_, float value_) : variable_t(name_, std::to_string(value_), 0, 0) {}
    variable_t(std::string name_, int32_t value_) : variable_t(name_, std::to_string(value_), 0, 0)
    {
    }

    virtual std::unique_ptr<operand_t> clone() const { return std::make_unique<variable_t>(*this); }

    std::string get_name() const { return name; }
    std::string get_value() const { return value; }

    void set_value(int32_t value_) { value = std::to_string(value_); }
    void set_value(uint32_t value_) { value = std::to_string(value_); }
    void set_value(float value_) { value = std::to_string(value_); }

    virtual variable_t operator()(uint32_t offset_) const
    {
        return variable_t(name, value, indices[0] + offset_);
    }
    virtual variable_t operator()(uint32_t offset_start_, uint32_t offset_end_) const
    {
        return variable_t(name, value, indices[0] + offset_start_, indices[0] + offset_end_);
    }

    void expr_name_with_indices(std::ostream& os_) const
    {
        // std::cout << "xxx!" << indices[0]<<", " << indices[1]<<std::endl;
        if(indices[0] == 0 && indices[1] == 0)
            os_ << name;
        else if(indices[0] == indices[1])
        {
            os_ << name << "+" << indices[0];
        }
        else
        {
            os_ << name << "+" << indices[0] << ":" << name << "+" << indices[1];
        }
    }

    virtual void expr(std::ostream& os_) const
    {
        // std::cout<< " expr var:"<<name<<", value"<<std::endl;
        if(name.find("v_", 0, 2) != std::string::npos)
        {
            os_ << "v[";
            expr_name_with_indices(os_);
            os_ << "]";
        }
        else if(name.find("s_", 0, 2) != std::string::npos)
        {
            os_ << "s[";
            expr_name_with_indices(os_);
            os_ << "]";
        }
        else if(name.find("a_", 0, 2) != std::string::npos)
        {
            os_ << "a[";
            expr_name_with_indices(os_);
            os_ << "]";
        }
        else
        {
            os_ << name;
        }
    }

private:
    // operand_t * ref_operand;
    std::string name;
    std::string value;   // we'll never use the value in codegen, just keep it as string
    uint32_t indices[2]; // this is the offset relent to this var. similiar to register
};
#define GXCO_VAR(name, value)              \
    gxco::variable_t name(#name, (value)); \
    register_variable(name)

class macro_args_t : public operand_t
{
public:
    macro_args_t(std::string name_, emitter_type emit_tp_, uint32_t i_start_, uint32_t i_end_)
        : operand_t(OPERAND_MACRO_ARG), name(name_), emit_tp(emit_tp_), indices{i_start_, i_end_}
    {
    }
    macro_args_t(std::string name_, emitter_type emit_tp_, uint32_t i_start_)
        : macro_args_t(name_, emit_tp_, i_start_, i_start_)
    {
    }
    macro_args_t(std::string name_, emitter_type emit_tp_) : macro_args_t(name_, emit_tp_, 0, 0) {}

    virtual std::unique_ptr<operand_t> clone() const
    {
        return std::make_unique<macro_args_t>(*this);
    }

    std::string get_name() const { return name; }

    macro_args_t operator()(uint32_t offset_) const { return macro_args_t(name, emit_tp, offset_); }
    macro_args_t operator()(uint32_t offset_start_, uint32_t offset_end_) const
    {
        return macro_args_t(name, emit_tp, offset_start_, offset_end_);
    }

    void expr_name_with_indices(std::ostream& os_) const
    {
        if(emit_tp == EMITTER_SP3)
        {
            if(indices[0] == 0 && indices[1] == 0)
                os_ << name;
            else if(indices[0] == indices[1])
            {
                os_ << name << "+" << indices[0];
            }
            else
            {
                os_ << name << "+" << indices[0] << ":" << name << "+" << indices[1];
            }
        }
        else
        {
            if(indices[0] == 0 && indices[1] == 0)
                os_ << "\\" << name;
            else if(indices[0] == indices[1])
            {
                os_ << "\\" << name << "+" << indices[0];
            }
            else
            {
                os_ << "\\" << name << "+" << indices[0] << ":"
                    << "\\" << name << "+" << indices[1];
            }
        }
    }

    virtual void expr(std::ostream& os_) const
    {
        if(name.find("v_", 0, 2) != std::string::npos)
        {
            os_ << "v[";
            expr_name_with_indices(os_);
            os_ << "]";
        }
        else if(name.find("s_", 0, 2) != std::string::npos)
        {
            os_ << "s[";
            expr_name_with_indices(os_);
            os_ << "]";
        }
        else if(name.find("a_", 0, 2) != std::string::npos)
        {
            os_ << "a[";
            expr_name_with_indices(os_);
            os_ << "]";
        }
        else
        {
            os_ << name;
        }
    }

private:
    std::string name;
    emitter_type emit_tp;
    uint32_t indices[2]; // this is the offset relent to this arg. similiar to register
};
#define GXCO_MACRO_ARG(name)                                       \
    gxco::macro_args_t name(#name, generator->get_emitter_type()); \
    register_arg(name)

const macro_args_t& operand_t::as_macro_arg() const
{
    assert(get_type() == OPERAND_MACRO_ARG);
    return static_cast<const macro_args_t&>(*this);
}
const variable_t& operand_t::as_variable() const
{
    assert(get_type() == OPERAND_VARIABLE);
    return static_cast<const variable_t&>(*this);
}
const vgpr_t& operand_t::as_vgpr() const
{
    assert(get_type() == OPERAND_REGISTER_VGPR);
    return static_cast<const vgpr_t&>(*this);
}
const sgpr_t& operand_t::as_sgpr() const
{
    assert(get_type() == OPERAND_REGISTER_SGPR);
    return static_cast<const sgpr_t&>(*this);
}
const agpr_t& operand_t::as_agpr() const
{
    assert(get_type() == OPERAND_REGISTER_AGPR);
    return static_cast<const agpr_t&>(*this);
}
const hwreg_t& operand_t::as_hwreg() const
{
    assert(get_type() == OPERAND_REGISTER_HWREG);
    return static_cast<const hwreg_t&>(*this);
}

#if 0
class register_pool_t {
public:
    std::vector<std::unique_ptr<operand_t>> & get() { return operands; }
    const std::vector<std::unique_ptr<operand_t>> & get() const { return operands; }
    operand_t * insert(const operand_t & operand_) {
        std::string key = get_key(operand_);
        if(map.count(key) == 0){
            // not found, create a new one
            map[key] = operand_.clone();
            if(operand_.get_type() == operand_t::OPERAND_VARIABLE){
                std::unique_ptr<operand_t> ref_op = operand_.create_reference_operand();
                std::string ref_key = get_key(*ref_op);
                if(map.count(ref_key) == 0){
                    map[ref_key] = operand_.clone();
                }
                operand_.as_variable().set_ref_operand(map[ref_key].get());
            }
        }

        return map[key].get();
    }

    // key is s_/v_/k_/a_ + the index. if hwreg, just the name of hwreg
    static std::string get_key(const operand_t & operand_) {
        switch(operand_.get_type()){
            case operand_t::OPERAND_REGISTER_SGPR:
                return std::string("SGPR_") + std::to_string(operand_.as_sgpr().get_index());
            break;
            case operand_t::OPERAND_REGISTER_VGPR:
                return std::string("VGPR_") + std::to_string(operand_.as_vgpr().get_index());
            break;
            case operand_t::OPERAND_REGISTER_AGPR:
                return std::string("AGPR_") + std::to_string(operand_.as_agpr().get_index());
            break;
            case operand_t::OPERAND_REGISTER_HWREG:
                return std::string("HWREG_") + std::string(operand_.as_hwreg().get_name());
            break;
            case operand_t::OPERAND_VARIABLE:
                return std::string("VAR_") + std::string(operand_.as_variable().get_name());
            break;
            default:
                assert(0);
            break;
        }
    }

private:
    std::unordered_map<std::string, std::unique_ptr<operand_t>> map;
};
#endif

class instruction_t
{
public:
#define GXCO_IIFE_OPERAND_REF_VEC(type_, var_)  \
    [&]() {                                     \
        std::vector<std::unique_ptr<type_>> x_; \
        for(auto& item_ : var_)                 \
            x_.push_back(item_.get().clone());  \
        return x_;                              \
    }()

#define GXCO_IIFE_OPERAND_VEC(type_, var_)      \
    [&]() {                                     \
        std::vector<std::unique_ptr<type_>> x_; \
        for(auto& item_ : var_)                 \
            x_.push_back(item_.clone());        \
        return x_;                              \
    }()

#define GXCO_IIFE_OPERAND_PTR(type_, var_)      \
    [&]() {                                     \
        std::vector<std::unique_ptr<type_>> x_; \
        for(auto& item_ : var_)                 \
            x_.push_back(item_->clone());       \
        return x_;                              \
    }()

    instruction_t(encoding enc_,
                  std::string name_,
                  const std::initializer_list<std::reference_wrapper<const operand_t>>& dst_,
                  const std::initializer_list<std::reference_wrapper<const operand_t>>& src_,
                  const std::initializer_list<modifier_t>& mod_,
                  uint32_t indent_level_)
        : enc(enc_),
          name(name_),
          dst(GXCO_IIFE_OPERAND_REF_VEC(operand_t, dst_)),
          src(GXCO_IIFE_OPERAND_REF_VEC(operand_t, src_)),
          modifier(GXCO_IIFE_OPERAND_VEC(modifier_t, mod_)),
          indent(indent_level_)
    {
        // dst_.begin()->get().expr(std::cout); dst[0]->expr(std::cout);
        // std::cout<<"xxx"<<std::endl;
    }

    instruction_t(const instruction_t& other_)
    {
        enc      = other_.enc;
        name     = other_.name;
        dst      = GXCO_IIFE_OPERAND_PTR(operand_t, other_.dst);
        src      = GXCO_IIFE_OPERAND_PTR(operand_t, other_.src);
        modifier = GXCO_IIFE_OPERAND_PTR(modifier_t, other_.modifier);
        indent   = other_.indent;
    }
    // instruction_t &operator=(const instruction_t&) = delete;

    encoding enc;
    std::string name;
    std::vector<std::unique_ptr<operand_t>> dst;
    std::vector<std::unique_ptr<operand_t>> src;
    std::vector<std::unique_ptr<modifier_t>> modifier;
    uint32_t indent;
};

class karg_t
{
public:
    karg_t(const std::string& name_,
           uint32_t size_,
           uint32_t offset_,
           const std::string& kind_,
           const std::string& type_,
           const std::string& address_space_ = "",
           bool is_const_                    = false)
        : name(name_),
          size(size_),
          offset(offset_),
          kind(kind_),
          type(type_),
          address_space(address_space_),
          is_const(is_const_)
    {
    }
    std::string name;
    uint32_t size;
    uint32_t offset;
    std::string kind;
    std::string type;
    std::string address_space;
    bool is_const;
};

// kernel resource
class kres_t
{
public:
    // descriptor
    uint32_t group_segment_fixed_size{0}; // LDS size in byte
    uint32_t private_segment_fixed_size{0};
    uint32_t kernarg_size{0};
    uint32_t kernel_code_entry_byte_offset{0};
    uint32_t enable_sgpr_private_segment_buffer{0};
    uint32_t enable_sgpr_dispatch_ptr{0};
    uint32_t enable_sgpr_queue_ptr{0};
    uint32_t enable_sgpr_kernarg_segment_ptr{0};
    uint32_t enable_sgpr_dispatch_id{0};
    uint32_t enable_sgpr_flat_scratch_init{0};
    uint32_t enable_sgpr_private_segment_size{0};
    uint32_t enable_wavefront_size32{0};
    uint32_t uses_dynamic_stack{0};

    // COMPUTE_PGM_RSRC1 related info
    uint32_t vgprs{0}; // total number of vgprs, not the GRANULATED_WORKITEM_VGPR_COUNT
    uint32_t sgprs{0}; // total number of sgprs, not the GRANULATED_WAVEFRONT_SGPR_COUNT
    uint32_t float_round_mode_32{0};
    uint32_t float_round_mode_16_64{0};
    uint32_t float_denorm_mode_32{0};
    uint32_t float_denorm_mode_16_64{0};
    uint32_t priv{0};
    uint32_t dx10_clamp{0};
    uint32_t debug_mode{0};
    uint32_t ieee_mode{0};
    uint32_t bulky{0};
    uint32_t cdbg_user{0};
    uint32_t fp16_ovfl{0};
    uint32_t wgp_mode{0};
    uint32_t mem_ordered{0};
    uint32_t fwd_progress{0};

    // COMPUTE_PGM_RSRC2 related info
    uint32_t enable_private_segment{0};
    uint32_t user_sgpr_count{0};
    uint32_t enable_trap_handler{0};
    uint32_t enable_sgpr_workgroup_id_x{0};
    uint32_t enable_sgpr_workgroup_id_y{0};
    uint32_t enable_sgpr_workgroup_id_z{0};
    uint32_t enable_sgpr_workgroup_info{0};
    uint32_t enable_vgpr_workitem_id{0};
    uint32_t enable_exception_address_watch{0};
    uint32_t enable_exception_memory{0};
    uint32_t granulated_lds_size{0}; // must be zero
    uint32_t enable_exception_ieee_754_fp_invalid_operation{0};
    uint32_t enable_exception_fp_denormal_source{0};
    uint32_t enable_exception_ieee_754_fp_division_by_zero{0};
    uint32_t enable_exception_ieee_754_fp_overflow{0};
    uint32_t enable_exception_ieee_754_fp_underflow{0};
    uint32_t enable_exception_ieee_754_fp_inexact{0};
    uint32_t enable_exception_int_divide_by_zero{0};

    // COMPUTE_PGM_RSRC3 for gfx90A, 940
    uint32_t accum_offset{0};
    uint32_t tg_split{0};

    // COMPUTE_PGM_RSRC3 for gfx10+
    uint32_t shared_vgpr_count{0};
    uint32_t inst_pref_size{0};
    uint32_t trap_on_start{0};
    uint32_t trap_on_end{0};
    uint32_t image_op{0};

    // extra information
    std::string kernel_name;
    uint32_t kernarg_alignment{0};
    std::array<uint32_t, 3> workgroup_size{0, 0, 0};

    // args
    std::vector<karg_t> kargs;
};

class generator_t;
class macro_t;
class emitter_t
{
public:
    emitter_t(std::ostream& os_, emitter_type type_) : os(os_), type(type_) {}
    virtual void emit_variable_declare(const operand_t& var_) const {}
    virtual void emit_macro_declare(const macro_t& macro_) const {}
    virtual void emit_instruction(const instruction_t& inst_) const
    {
        // std::cout<<"## inst:"<<inst_.name<<", dst:"<<inst_.dst.size()<<",
        // src:"<<inst_.src.size()<<std::endl;
        if(inst_.enc == ENC_FAKE_COMMENT_LINE)
        {
            // TODO: implement?
            return;
        }
        if(inst_.enc == ENC_FAKE_NEW_LINE)
        {
            os << std::endl;
            return;
        }
        if(inst_.enc == ENC_FAKE_LABEL)
        {
            os << inst_.name << ":" << std::endl; // label define, never have indent
            return;
        }
        if(inst_.enc == ENC_MACRO)
        {
            if(type == EMITTER_SP3)
            {
                os << std::string(inst_.indent, ' ');
                os << inst_.name << "(";
                uint32_t arg_cnt = 0;
                for(const auto& arg_ : inst_.dst)
                {
                    os << ((arg_cnt > 0) ? ", " : "") << arg_->as_variable().get_name();
                    arg_cnt++;
                }
                os << ")";
                os << std::endl;
            }
            else
            {
                os << std::string(inst_.indent, ' ');
                os << inst_.name;
                uint32_t arg_cnt = 0;
                for(const auto& arg_ : inst_.dst)
                {
                    os << ((arg_cnt > 0) ? ", " : " ") << arg_->as_variable().get_name();
                    arg_cnt++;
                }
                os << std::endl;
            }
            os.flush();
            return;
        }
        os << std::string(inst_.indent, ' ');
        os << inst_.name;

        // print operand
        uint32_t operand_cnt      = 0;
        std::string opr_delimiter = " ";
        auto update_opr_delimiter = [&]() {
            if(operand_cnt > 0)
                opr_delimiter = ", ";
        };

        auto emit_operand_vec = [&](const std::vector<std::unique_ptr<operand_t>>& opr_vec) {
            for(const auto& operand : opr_vec)
            {
                int32_t modifier_level = 0;
                // std::cout<<"  type:"<<operand->get_type()<<std::endl;
                if(operand->get_type() != OPERAND_UNUSED)
                {
                    // emit operand modifier
                    for(const auto& m : operand->get_modifier())
                    {
                        os << m.get_name() << "(";
                        modifier_level++;
                    }
                    os << opr_delimiter;
                    operand->expr(os);
                    operand_cnt++;

                    for(int32_t ml = modifier_level; ml > 0; ml--)
                    {
                        os << ")";
                    }
                }
                update_opr_delimiter();
            }
        };

        emit_operand_vec(inst_.dst);
        emit_operand_vec(inst_.src);
        for(const auto& m : inst_.modifier)
        {
            os << " ";
            m->expr(os);
        }
        os << std::endl;
        os.flush();
    }

    virtual void emit(const generator_t& generator) const = 0;
    virtual void emit_dv(const gxco::kres_t& name,
                         std::vector<std::tuple<std::string, std::string>>) const {};

    virtual void emit_macro(const generator_t& generator) const {}
    virtual void emit_variable(const generator_t& generator) const {}
    virtual void emit_kernel_code(const generator_t& generator) const {}

    virtual void emit_metadata_per_kernel_to_buf(const generator_t& generator,
                                                 std::stringstream& ss_buf) const
    {
    }
    virtual void emit_metadata_from_buf(const generator_t& generator,
                                        std::stringstream& ss_buf) const
    {
    }

    emitter_type get_emitter_type() const { return type; }

protected:
    std::ostream& os;
    emitter_type type;
};

static inline auto operand_modify_abs(const operand_t& op_)
{
    auto new_op = op_.clone();
    new_op->modify(modifier_t{MODIFIER_ABS});
    return new_op;
}
#define m_abs(op_) (*operand_modify_abs(op_))

static inline auto operand_modify_neg(const operand_t& op_)
{
    auto new_op = op_.clone();
    new_op->modify(modifier_t{MODIFIER_NEG});
    return new_op;
}
#define m_neg(op_) (*operand_modify_neg(op_))

#include "gxco_isa.h"

class macro_t : public gxco_isa_t
{
public:
    macro_t(const target_info_t& ti_,
            emitter_t& emitter_,
            gxco_isa_t* generator_,
            std::string name_)
        : gxco_isa_t(ti_, emitter_), generator(generator_), name(name_)
    {
    }

    std::unique_ptr<macro_t> clone() const { return std::make_unique<macro_t>(*this); }

    template <typename... Arguments>
    void operator()(const Arguments&... macro_args_)
    {
        generator->record(
            instruction_t(ENC_MACRO, name, {macro_args_...}, {}, {}, generator->get_indent()));
    }

    std::string get_name() const { return name; }
    void register_arg(const macro_args_t& arg_) { args.push_back(arg_); }
    const std::vector<macro_args_t>& get_args() const { return args; }

protected:
    gxco_isa_t* generator;
    std::string name;
    std::vector<macro_args_t> args;
};

#define GXCO_MACRO(name, args, code)                          \
    class name##_macro : public gxco::macro_t                 \
    {                                                         \
    public:                                                   \
        name##_macro(const gxco::target_info_t& ti_,          \
                     gxco::emitter_t& emitter_,               \
                     gxco_isa_t* generator_,                  \
                     std::string name_)                       \
            : gxco::macro_t(ti_, emitter_, generator_, name_) \
        {                                                     \
            update_indent(4);                                 \
            args code                                         \
        }                                                     \
    } name{ti_, emitter_, this, #name};                       \
    register_macro(name)

// inherit from this class to generate your code
class generator_t : public gxco_isa_t
{
public:
    generator_t(const target_info_t& ti_, emitter_t& emitter_) : gxco_isa_t(ti_, emitter_) {}

    void register_variable(const operand_t& var_)
    {
        assert(var_.get_type() == OPERAND_VARIABLE);
        variabls.push_back(var_.clone());
    }
    void register_macro(const macro_t& macro_) { macro_sequence.push_back(macro_.clone()); }

    const std::vector<std::unique_ptr<macro_t>>& get_macro_sequence() const
    {
        return macro_sequence;
    }
    const std::vector<std::unique_ptr<operand_t>>& get_variables() const { return variabls; }

    const kres_t& get_kres() const { return kres; }
    kres_t& get_kres() { return kres; }

    void emit() { emitter.emit(*this); }

protected:
    std::vector<std::unique_ptr<macro_t>> macro_sequence;
    std::vector<std::unique_ptr<operand_t>> variabls;

    kres_t kres;
};

// emit as llvm assembly, and use llvm assembler to compile
class emit_llvm_hsa_t : public emitter_t
{
public:
    emit_llvm_hsa_t(std::ostream& os_, hsa_code_object_version cov_)
        : emitter_t(os_, EMITTER_LLVM_HSA), cov(cov_)
    {
    }
    virtual void emit_variable_declare(const operand_t& var_) const
    {
        assert(var_.get_type() == OPERAND_VARIABLE);
        const auto& v = var_.as_variable();
        os << ".set " << v.get_name() << ", " << v.get_value() << "\n";
    }

    virtual void emit_macro_declare(const macro_t& macro_) const
    {
        os << ".macro " << macro_.get_name();
        uint32_t arg_count = 0;
        for(const auto& arg_ : macro_.get_args())
        {
            os << ((arg_count > 0) ? ", " : " ") << arg_.get_name();
            arg_count++;
        }
        os << std::endl;
    }

    virtual void emit_single_karg(const karg_t& karg, std::ostream& os_) const
    {
        os_ << "- { ";
        os_ << ".name: " << karg.name << ", .size: " << karg.size << ", .offset: " << karg.offset
            << ", .value_kind: " << karg.kind << ", .value_type: " << karg.type;
        if(karg.address_space != "")
        {
            os_ << ", .address_space: " << karg.address_space
                << ", .is_const: " << (karg.is_const ? "true" : "false");
        }
        os_ << "}";
    }

    // 1st, emit some macro
    virtual void emit_macro(const generator_t& generator) const
    {
        for(const auto& macro_ : generator.get_macro_sequence())
        {
            emit_macro_declare(*macro_);
            for(const auto& inst : macro_->get_instruction_sequence())
            {
                emit_instruction(inst);
            }
            os << ".endm" << std::endl;
            os << std::endl;
        }
    }

    // 2st, emit variable declaration
    virtual void emit_variable(const generator_t& generator) const
    {
        for(const auto& var : generator.get_variables())
        {
            emit_variable_declare(*var);
        }
        os << std::endl;
    }
    virtual void emit_kernel_code(const generator_t& generator) const
    {
        const auto& kres        = generator.get_kres();
        std::string kernel_name = kres.kernel_name;
        const auto& ti          = generator.get_target_info();

        os << ".text" << std::endl;
        os << ".global " << kernel_name << std::endl;
        os << ".p2align 8" << std::endl;
        os << ".type " << kernel_name << ",@function" << std::endl;
        os << kernel_name << ":" << std::endl;
        for(const auto& inst : generator.get_instruction_sequence())
        {
            emit_instruction(inst);
        }

        // 4th, emit metadata
        os << ".rodata" << std::endl;
        os << ".p2align 6" << std::endl;
        os << ".amdhsa_kernel " << kernel_name << std::endl;
        os << "    .amdhsa_group_segment_fixed_size " << kres.group_segment_fixed_size << std::endl;
        os << "    .amdhsa_user_sgpr_dispatch_ptr " << kres.enable_sgpr_dispatch_ptr << std::endl;
        os << "    .amdhsa_user_sgpr_kernarg_segment_ptr " << kres.enable_sgpr_kernarg_segment_ptr
           << std::endl;
        if(kres.enable_sgpr_workgroup_id_x > 0)
            os << "    .amdhsa_system_sgpr_workgroup_id_x " << kres.enable_sgpr_workgroup_id_x
               << std::endl;
        if(kres.enable_sgpr_workgroup_id_y > 0)
            os << "    .amdhsa_system_sgpr_workgroup_id_y " << kres.enable_sgpr_workgroup_id_y
               << std::endl;
        if(kres.enable_sgpr_workgroup_id_z > 0)
            os << "    .amdhsa_system_sgpr_workgroup_id_z " << kres.enable_sgpr_workgroup_id_z
               << std::endl;
        os << "    .amdhsa_system_vgpr_workitem_id " << kres.enable_vgpr_workitem_id << std::endl;
        os << "    .amdhsa_next_free_vgpr " << kres.vgprs << std::endl;
        os << "    .amdhsa_next_free_sgpr " << kres.sgprs << std::endl;
        os << "    .amdhsa_ieee_mode " << kres.ieee_mode << std::endl;
        os << "    .amdhsa_dx10_clamp " << kres.dx10_clamp << std::endl;
        if(ti.arch >= 910 && ti.arch < 1000)
        {
            os << "    .amdhsa_tg_split " << kres.tg_split << std::endl;
            os << "    .amdhsa_accum_offset " << kres.accum_offset << std::endl;
        }
        if(ti.arch >= 1000)
        {
            os << "    .amdhsa_wavefront_size32 " << kres.enable_wavefront_size32 << std::endl;
            os << "    .amdhsa_workgroup_processor_mode " << kres.wgp_mode << std::endl;
        }
        os << ".end_amdhsa_kernel" << std::endl;
        os << std::endl;
    }

    virtual void emit_metadata_per_kernel_to_buf(const generator_t& generator,
                                                 std::stringstream& ss_buf) const
    {
        const auto& kres        = generator.get_kres();
        std::string kernel_name = kres.kernel_name;

        ss_buf << "  - .name: " << kernel_name << std::endl;
        ss_buf << "    .symbol: " << kernel_name << ".kd" << std::endl;
        ss_buf << "    .sgpr_count: " << kres.sgprs + 6 << std::endl; // VCC + SCRATCH + XNACK
        ss_buf << "    .vgpr_count: " << kres.vgprs << std::endl;
        ss_buf << "    .kernarg_segment_align: " << kres.kernarg_alignment << std::endl;
        ss_buf << "    .kernarg_segment_size: " << kres.kernarg_size << std::endl;
        ss_buf << "    .group_segment_fixed_size: " << kres.group_segment_fixed_size << std::endl;
        ss_buf << "    .private_segment_fixed_size: " << kres.private_segment_fixed_size
               << std::endl;
        ss_buf << "    .wavefront_size: " << (kres.enable_wavefront_size32 ? 32 : 64) << std::endl;
        ss_buf << "    .reqd_workgroup_size : [" << kres.workgroup_size[0] << ", "
               << kres.workgroup_size[1] << ", " << kres.workgroup_size[2] << "]" << std::endl;
        ss_buf << "    .max_flat_workgroup_size: "
               << kres.workgroup_size[0] * kres.workgroup_size[1] * kres.workgroup_size[2]
               << std::endl;
        ss_buf << "    .args:" << std::endl;
        for(const auto& karg : kres.kargs)
        {
            ss_buf << "    ";
            emit_single_karg(karg, ss_buf);
            ss_buf << std::endl;
        }
    }

    virtual void emit_metadata_from_buf(std::stringstream& ss_buf) const
    {
        os << ".amdgpu_metadata" << std::endl;
        os << "---" << std::endl;
        os << "amdhsa.version: [ 1, 0 ]" << std::endl;
        os << "amdhsa.kernels:" << std::endl;
        os << ss_buf.str();
        os << "..." << std::endl;
        os << ".end_amdgpu_metadata" << std::endl;
    }

    virtual void emit(const generator_t&) const
    { /* do not use this directly */
    }

private:
    hsa_code_object_version cov;
};

/**
 * @brief sp3 and associate componets emitter, develop for one_model/gfx11@AM
 *
 * @param os_ output stream
 * @param ShaderOnly bool variable, determine emit associate componets or not
 *
 * @details
 * Shader               *.sp3
 * (User Interface)     test.dv
 * (Compile Interface)  album.dj
 */
class emit_sp3_t : public emitter_t
{
public:
    emit_sp3_t(std::ostream& shader_os_,
               std::ostream& dj_os_,
               std::ostream& dv_os_,
               bool ShaderOnly_)
        : emitter_t(shader_os_, EMITTER_SP3), dj_os(dj_os_), dv_os(dv_os_), ShaderOnly(ShaderOnly_)
    {
    }

    virtual void emit_macro_declare(const macro_t& macro_) const
    {
        os << "function " << macro_.get_name() << "(";
        uint32_t arg_count = 0;
        for(const auto& arg_ : macro_.get_args())
        {
            os << ((arg_count > 0) ? ", " : "") << arg_.get_name();
            arg_count++;
        }
        os << ")" << std::endl;
    }

    virtual void emit_variable_declare(const operand_t& var_) const
    {
        assert(var_.get_type() == OPERAND_VARIABLE);
        const auto& v = var_.as_variable();
        os << "var " << v.get_name() << "= " << v.get_value() << "\n";
    }

    void emit_dj(std::string& kernel_name) const
    {

        dj_os << "component :gc do |variant|" << std::endl;
        dj_os << "    standard_design_comp_build :test_suites_block_" << kernel_name << " do"
              << std::endl;
        dj_os << "    need cdl::gen_envs" << std::endl;
        dj_os << "    need tbhydra::cmntest" << std::endl;
        dj_os << "    need tbhydra::transapi" << std::endl;
        dj_os << "    need tbhydra::testctrl" << std::endl;
        dj_os << "    need register::headers" << std::endl;
        dj_os << "    need gc::reg_headers" << std::endl;
        dj_os << "" << std::endl;
        dj_os << "        need flows" << std::endl;
        dj_os << "        need generic" << std::endl;
        dj_os << "        need utility" << std::endl;
        dj_os << "        need test_tools_core_init" << std::endl;
        dj_os << "        need test_tools_block_pm4lib" << std::endl;
        dj_os << "    end" << std::endl;
        dj_os << "end" << std::endl;
    }

    virtual void emit_dv(const gxco::kres_t& kres,
                         std::vector<std::tuple<std::string, std::string>> cmd_args) const override
    {

        std::string name = "kl_" + kres.kernel_name;

        dv_os << "test template " << name << "_test_t : perf_pm4lib_test" << std::endl;
        dv_os << "    when = " << name << std::endl;
        dv_os << "    group = kl/" << name << std::endl;
        dv_os << "    gen = " << name << std::endl;
        dv_os << "    args += shaderPath=ANCHOR_gc/src/test/suites/block/kl/" << name << "/shaders/"
              << std::endl;
        dv_os << "" << std::endl;
        dv_os
            << "    args += -tc_MaxPollCycles=10000000 -tR600DumpFixedFunctionVertexShader=disable "
               "-tR600DumpFixedFunctionPixelShader=disable -tR600UseConstFields"
            << std::endl;
        dv_os << "    args += -tcore_log=enabled" << std::endl;
        dv_os << "    args += -WGP_NUM=eval(GC__NUM_SE * GC__NUM_SA_PER_SE * GC__NUM_WGP_PER_SA)"
              << std::endl;
        dv_os << "    args += -pm4lib_MAX_TIMEOUT = 10000000" << std::endl;
        dv_os << "    args += bulky=0" << std::endl;
        dv_os << "" << std::endl;
        dv_os << "    tool.sim_run.args += -run_opts +maxSimCycles=200000000" << std::endl;
        dv_os << "    tool.sim_run.args += +all_trackers=off" << std::endl;
        dv_os << "    tool.sim_run.args += \"+ssm_max_cycles=320000000\"" << std::endl;
        dv_os << "" << std::endl;
        dv_os << "    tool.emu_gen_ini.args +=\"-model_args=-max_clocks=500000000\"" << std::endl;
        dv_os << "    tool.chip_gfx_mem_diff.args += -neverfail" << std::endl;
        dv_os << "endtest" << std::endl;

        // One test
        dv_os << "" << std::endl;
        dv_os << "test pm4_" << name << ":" << name << "_test_t" << std::endl;

        dv_os << "    args += blockSize=" << kres.workgroup_size[0] << std::endl;
        dv_os << "    args += lds_size=" << (kres.group_segment_fixed_size + 512 - 1) / 512
              << std::endl;
        for(auto iter = cmd_args.rbegin(); iter != cmd_args.rend(); iter++)
        {
            dv_os << "    args += " << std::get<0>(*iter) << "=" << std::get<1>(*iter) << std::endl;
        }
        dv_os << "    args += cs0=" << name.substr(name.find_first_of("_") + 1, name.length())
              << ".sp3" << std::endl;
        dv_os << "endtest" << std::endl;
    }

    virtual void emit(const generator_t& generator) const
    {
        const auto& kres        = generator.get_kres();
        std::string kernel_name = "kl_" + kres.kernel_name;
        // const auto & ti = generator.get_target_info();

        if(!ShaderOnly)
        {
            emit_dj(kernel_name);
        }

        // 1st, emit some function
        for(const auto& macro_ : generator.get_macro_sequence())
        {
            emit_macro_declare(*macro_);
            for(const auto& inst : macro_->get_instruction_sequence())
            {
                emit_instruction(inst);
            }
            os << "end" << std::endl;
            os << std::endl;
        }

        // 2nd, emit variable declaration
        for(const auto& var : generator.get_variables())
        {
            emit_variable_declare(*var);
        }

        os << std::endl;

        // 3rd, emit kernel code
        os << "shader main" << std::endl;
        os << "    type(CS)" << std::endl;
        os << "    wave_size(32)" << std::endl;
        os << std::endl;
        for(const auto& inst : generator.get_instruction_sequence())
        {
            emit_instruction(inst);
        }

        // 4th, emit end mark
        os << "end" << std::endl;
    }

protected:
    std::ostream& dj_os;
    std::ostream& dv_os;
    bool ShaderOnly = 1;
};

} // namespace gxco

#endif
