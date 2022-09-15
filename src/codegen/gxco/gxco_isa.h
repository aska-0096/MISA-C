class gxco_isa_t {
public:
    gxco_isa_t(const target_info_t & ti_, emitter_t & emitter_) : ti(ti_), emitter(emitter_), indent(0) {}

    //vgpr_t v0(0), v1(1), v2(2), v3(3), v4(4), v5(5), v6(6), v7(7), v8(8), v(9), v10(10);
    hwreg_t scc{"scc"}, vcc{"vcc"}, vcc_hi{"vcc_hi"}, vcc_lo{"vcc_lo"}, exec{"exec"}, exec_hi{"exec_hi"}, exec_lo{"exec_lo"}, off{"off"};
    vgpr_t v0{0}, v1{1}, v2{2}, v3{3}, v4{4}, v5{5}, v6{6}, v7{7}, v8{8}, v9{9}, v10{10};
    waitcnt_t waitcnt(uint32_t cnt_)  { return waitcnt_t{cnt_};}
    waitcnt_t vmcnt(uint32_t v_)    { return waitcnt_t{VMCNT(v_  ) | LGKMCNT(0xff) | EXPCNT(0xff) | VSCNT(0xff)};}
    waitcnt_t lgkmcnt(uint32_t v_)  { return waitcnt_t{VMCNT(0xff) | LGKMCNT(v_  ) | EXPCNT(0xff) | VSCNT(0xff)};}
    waitcnt_t expcnt(uint32_t v_)   { return waitcnt_t{VMCNT(0xff) | LGKMCNT(0xff) | EXPCNT(v_  ) | VSCNT(0xff)};}
    waitcnt_t vscnt(uint32_t v_)    { return waitcnt_t{VMCNT(0xff) | LGKMCNT(0xff) | EXPCNT(0xff) | VSCNT(v_  )};}
    label_t label(std::string label_name_) { insert_label(label_name_); return label_t{label_name_}; };    // immediately insert into instruction as label type, and return an instance

    // modifiers
    modifier_t clamp() {return modifier_t("clamp");}
    modifier_t offset(int32_t offset_) {return modifier_t("offset", offset_);}
    modifier_t offset(int32_t offset0_, int32_t offset1_) {return modifier_t("offset", {offset0_, offset1_});}
    // modifier - dpp
    modifier_t dpp8(int32_t l0, int32_t l1, int32_t l2, int32_t l3, int32_t l4, int32_t l5, int32_t l6, int32_t l7) {return modifier_t("dpp8", {l0,l1,l2,l3,l4,l5,l6,l7});}
    modifier_t quad_perm(int32_t l0, int32_t l1, int32_t l2, int32_t l3) {return modifier_t("quad_perm", {l0,l1,l2,l3});}
    modifier_t row_shl(int32_t lane) {return modifier_t("row_shl", lane);}
    modifier_t row_shr(int32_t lane) {return modifier_t("row_shr", lane);}
    modifier_t row_ror(int32_t lane) {return modifier_t("row_ror", lane);}
    modifier_t row_newbcast(int32_t lane) {return modifier_t("row_newbcast", lane);}
    modifier_t row_mask(uint32_t mask) {return modifier_t("row_mask", mask);}
    modifier_t bank_mask(uint32_t mask) {return modifier_t("bank_mask", mask);}
    // modifier - sdwa
    modifier_t dst_sel(std::string symbol_) {return modifier_t("dst_sel", symbol_);}
    modifier_t dst_unused(std::string symbol_) {return modifier_t("dst_unused", symbol_);}
    modifier_t src0_sel(std::string symbol_) {return modifier_t("src0_sel", symbol_);}
    modifier_t src1_sel(std::string symbol_) {return modifier_t("src1_sel", symbol_);}

    void update_indent(uint32_t indent_level_) {indent = indent_level_; }
    void insert_new_line() {record(instruction_t(ENC_FAKE_NEW_LINE, "", {}, {}, {}, indent));}
    void insert_label(std::string label_name_) {record(instruction_t(ENC_FAKE_LABEL, label_name_, {}, {}, {}, indent)); }

    // DS
    void ds_add_f32(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_add_f32", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_add_rtn_f32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_add_rtn_f32", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_add_rtn_u32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_add_rtn_u32", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_add_rtn_u64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_add_rtn_u64", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_add_u32(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_add_u32", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_add_u64(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_add_u64", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_and_b32(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_and_b32", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_and_b64(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_and_b64", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_and_rtn_b32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_and_rtn_b32", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_and_rtn_b64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_and_rtn_b64", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_append(const operand_t & vdst, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_append", vdst, operand_t(), operand_t(), operand_t(), modifier); }
    void ds_bpermute_b32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_bpermute_b32", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_cmpst_b32(const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_cmpst_b32", operand_t(), vaddr, vdata0, vdata1, modifier); }
    void ds_cmpst_b64(const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_cmpst_b64", operand_t(), vaddr, vdata0, vdata1, modifier); }
    void ds_cmpst_f32(const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_cmpst_f32", operand_t(), vaddr, vdata0, vdata1, modifier); }
    void ds_cmpst_f64(const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_cmpst_f64", operand_t(), vaddr, vdata0, vdata1, modifier); }
    void ds_cmpst_rtn_b32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_cmpst_rtn_b32", vdst, vaddr, vdata0, vdata1, modifier); }
    void ds_cmpst_rtn_b64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_cmpst_rtn_b64", vdst, vaddr, vdata0, vdata1, modifier); }
    void ds_cmpst_rtn_f32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_cmpst_rtn_f32", vdst, vaddr, vdata0, vdata1, modifier); }
    void ds_cmpst_rtn_f64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_cmpst_rtn_f64", vdst, vaddr, vdata0, vdata1, modifier); }
    void ds_condxchg32_rtn_b64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_condxchg32_rtn_b64", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_consume(const operand_t & vdst, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_consume", vdst, operand_t(), operand_t(), operand_t(), modifier); }
    void ds_dec_rtn_u32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_dec_rtn_u32", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_dec_rtn_u64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_dec_rtn_u64", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_dec_u32(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_dec_u32", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_dec_u64(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_dec_u64", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_gws_barrier(const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_gws_barrier", operand_t(), vdata, operand_t(), operand_t(), modifier); }
    void ds_gws_init(const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_gws_init", operand_t(), vdata, operand_t(), operand_t(), modifier); }
    void ds_gws_sema_br(const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_gws_sema_br", operand_t(), vdata, operand_t(), operand_t(), modifier); }
    void ds_gws_sema_p(const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_gws_sema_p", operand_t(), operand_t(), operand_t(), operand_t(), modifier); }
    void ds_gws_sema_release_all(const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_gws_sema_release_all", operand_t(), operand_t(), operand_t(), operand_t(), modifier); }
    void ds_gws_sema_v(const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_gws_sema_v", operand_t(), operand_t(), operand_t(), operand_t(), modifier); }
    void ds_inc_rtn_u32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_inc_rtn_u32", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_inc_rtn_u64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_inc_rtn_u64", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_inc_u32(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_inc_u32", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_inc_u64(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_inc_u64", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_max_f32(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_max_f32", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_max_f64(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_max_f64", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_max_i32(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_max_i32", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_max_i64(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_max_i64", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_max_rtn_f32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_max_rtn_f32", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_max_rtn_f64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_max_rtn_f64", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_max_rtn_i32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_max_rtn_i32", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_max_rtn_i64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_max_rtn_i64", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_max_rtn_u32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_max_rtn_u32", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_max_rtn_u64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_max_rtn_u64", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_max_u32(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_max_u32", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_max_u64(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_max_u64", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_min_f32(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_min_f32", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_min_f64(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_min_f64", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_min_i32(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_min_i32", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_min_i64(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_min_i64", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_min_rtn_f32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_min_rtn_f32", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_min_rtn_f64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_min_rtn_f64", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_min_rtn_i32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_min_rtn_i32", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_min_rtn_i64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_min_rtn_i64", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_min_rtn_u32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_min_rtn_u32", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_min_rtn_u64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_min_rtn_u64", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_min_u32(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_min_u32", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_min_u64(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_min_u64", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_mskor_b32(const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_mskor_b32", operand_t(), vaddr, vdata0, vdata1, modifier); }
    void ds_mskor_b64(const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_mskor_b64", operand_t(), vaddr, vdata0, vdata1, modifier); }
    void ds_mskor_rtn_b32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_mskor_rtn_b32", vdst, vaddr, vdata0, vdata1, modifier); }
    void ds_mskor_rtn_b64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_mskor_rtn_b64", vdst, vaddr, vdata0, vdata1, modifier); }
    void ds_nop() { r_ds("ds_nop", operand_t(), operand_t(), operand_t(), operand_t()); }
    void ds_or_b32(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_or_b32", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_or_b64(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_or_b64", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_or_rtn_b32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_or_rtn_b32", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_or_rtn_b64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_or_rtn_b64", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_ordered_count(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_ordered_count", vdst, vaddr, operand_t(), operand_t(), modifier); }
    void ds_permute_b32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_permute_b32", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_read2_b32(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_read2_b32", vdst, vaddr, operand_t(), operand_t(), modifier); }
    void ds_read2_b64(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_read2_b64", vdst, vaddr, operand_t(), operand_t(), modifier); }
    void ds_read2st64_b32(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_read2st64_b32", vdst, vaddr, operand_t(), operand_t(), modifier); }
    void ds_read2st64_b64(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_read2st64_b64", vdst, vaddr, operand_t(), operand_t(), modifier); }
    void ds_read_addtid_b32(const operand_t & vdst, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_read_addtid_b32", vdst, operand_t(), operand_t(), operand_t(), modifier); }
    void ds_read_b128(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_read_b128", vdst, vaddr, operand_t(), operand_t(), modifier); }
    void ds_read_b32(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_read_b32", vdst, vaddr, operand_t(), operand_t(), modifier); }
    void ds_read_b64(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_read_b64", vdst, vaddr, operand_t(), operand_t(), modifier); }
    void ds_read_b96(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_read_b96", vdst, vaddr, operand_t(), operand_t(), modifier); }
    void ds_read_i16(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_read_i16", vdst, vaddr, operand_t(), operand_t(), modifier); }
    void ds_read_i8(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_read_i8", vdst, vaddr, operand_t(), operand_t(), modifier); }
    void ds_read_i8_d16(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_read_i8_d16", vdst, vaddr, operand_t(), operand_t(), modifier); }
    void ds_read_i8_d16_hi(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_read_i8_d16_hi", vdst, vaddr, operand_t(), operand_t(), modifier); }
    void ds_read_u16(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_read_u16", vdst, vaddr, operand_t(), operand_t(), modifier); }
    void ds_read_u16_d16(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_read_u16_d16", vdst, vaddr, operand_t(), operand_t(), modifier); }
    void ds_read_u16_d16_hi(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_read_u16_d16_hi", vdst, vaddr, operand_t(), operand_t(), modifier); }
    void ds_read_u8(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_read_u8", vdst, vaddr, operand_t(), operand_t(), modifier); }
    void ds_read_u8_d16(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_read_u8_d16", vdst, vaddr, operand_t(), operand_t(), modifier); }
    void ds_read_u8_d16_hi(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_read_u8_d16_hi", vdst, vaddr, operand_t(), operand_t(), modifier); }
    void ds_rsub_rtn_u32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_rsub_rtn_u32", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_rsub_rtn_u64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_rsub_rtn_u64", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_rsub_u32(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_rsub_u32", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_rsub_u64(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_rsub_u64", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_sub_rtn_u32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_sub_rtn_u32", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_sub_rtn_u64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_sub_rtn_u64", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_sub_u32(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_sub_u32", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_sub_u64(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_sub_u64", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_swizzle_b32(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_swizzle_b32", vdst, vaddr, operand_t(), operand_t(), modifier); }
    void ds_wrap_rtn_b32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_wrap_rtn_b32", vdst, vaddr, vdata0, vdata1, modifier); }
    void ds_write2_b32(const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_write2_b32", operand_t(), vaddr, vdata0, vdata1, modifier); }
    void ds_write2_b64(const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_write2_b64", operand_t(), vaddr, vdata0, vdata1, modifier); }
    void ds_write2st64_b32(const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_write2st64_b32", operand_t(), vaddr, vdata0, vdata1, modifier); }
    void ds_write2st64_b64(const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_write2st64_b64", operand_t(), vaddr, vdata0, vdata1, modifier); }
    void ds_write_addtid_b32(const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_write_addtid_b32", operand_t(), vdata, operand_t(), operand_t(), modifier); }
    void ds_write_b128(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_write_b128", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_write_b16(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_write_b16", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_write_b16_d16_hi(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_write_b16_d16_hi", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_write_b32(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_write_b32", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_write_b64(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_write_b64", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_write_b8(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_write_b8", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_write_b8_d16_hi(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_write_b8_d16_hi", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_write_b96(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_write_b96", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_wrxchg2_rtn_b32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_wrxchg2_rtn_b32", vdst, vaddr, vdata0, vdata1, modifier); }
    void ds_wrxchg2_rtn_b64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_wrxchg2_rtn_b64", vdst, vaddr, vdata0, vdata1, modifier); }
    void ds_wrxchg2st64_rtn_b32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_wrxchg2st64_rtn_b32", vdst, vaddr, vdata0, vdata1, modifier); }
    void ds_wrxchg2st64_rtn_b64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata0, const operand_t & vdata1, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_wrxchg2st64_rtn_b64", vdst, vaddr, vdata0, vdata1, modifier); }
    void ds_wrxchg_rtn_b32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_wrxchg_rtn_b32", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_wrxchg_rtn_b64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_wrxchg_rtn_b64", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_xor_b32(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_xor_b32", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_xor_b64(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_xor_b64", operand_t(), vaddr, vdata, operand_t(), modifier); }
    void ds_xor_rtn_b32(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_xor_rtn_b32", vdst, vaddr, vdata, operand_t(), modifier); }
    void ds_xor_rtn_b64(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) { r_ds("ds_xor_rtn_b64", vdst, vaddr, vdata, operand_t(), modifier); }
    

    // FLAT
    void flat_atomic_add(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_add", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_add_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_add_x2", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_and(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_and", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_and_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_and_x2", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_cmpswap(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_cmpswap", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_cmpswap_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_cmpswap_x2", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_dec(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_dec", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_dec_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_dec_x2", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_fcmpswap(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_fcmpswap", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_fcmpswap_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_fcmpswap_x2", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_fmax(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_fmax", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_fmax_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_fmax_x2", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_fmin(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_fmin", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_fmin_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_fmin_x2", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_inc(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_inc", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_inc_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_inc_x2", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_or(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_or", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_or_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_or_x2", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_smax(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_smax", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_smax_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_smax_x2", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_smin(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_smin", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_smin_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_smin_x2", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_sub(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_sub", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_sub_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_sub_x2", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_swap(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_swap", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_swap_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_swap_x2", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_umax(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_umax", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_umax_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_umax_x2", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_umin(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_umin", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_umin_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_umin_x2", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_xor(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_xor", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_atomic_xor_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_atomic_xor_x2", vdst, vaddr, vdata, operand_t(), modifier);}
    void flat_load_dword(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_load_dword", vdst, vaddr, operand_t(), operand_t(), modifier);}
    void flat_load_dwordx2(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_load_dwordx2", vdst, vaddr, operand_t(), operand_t(), modifier);}
    void flat_load_dwordx3(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_load_dwordx3", vdst, vaddr, operand_t(), operand_t(), modifier);}
    void flat_load_dwordx4(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_load_dwordx4", vdst, vaddr, operand_t(), operand_t(), modifier);}
    void flat_load_sbyte(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_load_sbyte", vdst, vaddr, operand_t(), operand_t(), modifier);}
    void flat_load_sbyte_d16(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_load_sbyte_d16", vdst, vaddr, operand_t(), operand_t(), modifier);}
    void flat_load_sbyte_d16_hi(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_load_sbyte_d16_hi", vdst, vaddr, operand_t(), operand_t(), modifier);}
    void flat_load_short_d16(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_load_short_d16", vdst, vaddr, operand_t(), operand_t(), modifier);}
    void flat_load_short_d16_hi(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_load_short_d16_hi", vdst, vaddr, operand_t(), operand_t(), modifier);}
    void flat_load_sshort(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_load_sshort", vdst, vaddr, operand_t(), operand_t(), modifier);}
    void flat_load_ubyte(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_load_ubyte", vdst, vaddr, operand_t(), operand_t(), modifier);}
    void flat_load_ubyte_d16(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_load_ubyte_d16", vdst, vaddr, operand_t(), operand_t(), modifier);}
    void flat_load_ubyte_d16_hi(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_load_ubyte_d16_hi", vdst, vaddr, operand_t(), operand_t(), modifier);}
    void flat_load_ushort(const operand_t & vdst, const operand_t & vaddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_load_ushort", vdst, vaddr, operand_t(), operand_t(), modifier);}
    void flat_store_byte(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_store_byte", operand_t(), vaddr, vdata, operand_t(), modifier);}
    void flat_store_byte_d16_hi(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_store_byte_d16_hi", operand_t(), vaddr, vdata, operand_t(), modifier);}
    void flat_store_dword(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_store_dword", operand_t(), vaddr, vdata, operand_t(), modifier);}
    void flat_store_dwordx2(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_store_dwordx2", operand_t(), vaddr, vdata, operand_t(), modifier);}
    void flat_store_dwordx3(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_store_dwordx3", operand_t(), vaddr, vdata, operand_t(), modifier);}
    void flat_store_dwordx4(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_store_dwordx4", operand_t(), vaddr, vdata, operand_t(), modifier);}
    void flat_store_short(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_store_short", operand_t(), vaddr, vdata, operand_t(), modifier);}
    void flat_store_short_d16_hi(const operand_t & vaddr, const operand_t & vdata, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("flat_store_short_d16_hi", operand_t(), vaddr, vdata, operand_t(), modifier);}
    void global_atomic_add(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_add", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_add_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_add_x2", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_and(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_and", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_and_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_and_x2", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_cmpswap(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_cmpswap", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_cmpswap_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_cmpswap_x2", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_csub(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_csub", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_dec(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_dec", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_dec_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_dec_x2", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_fcmpswap(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_fcmpswap", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_fcmpswap_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_fcmpswap_x2", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_fmax(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_fmax", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_fmax_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_fmax_x2", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_fmin(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_fmin", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_fmin_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_fmin_x2", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_inc(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_inc", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_inc_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_inc_x2", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_or(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_or", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_or_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_or_x2", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_smax(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_smax", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_smax_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_smax_x2", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_smin(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_smin", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_smin_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_smin_x2", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_sub(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_sub", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_sub_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_sub_x2", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_swap(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_swap", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_swap_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_swap_x2", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_umax(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_umax", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_umax_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_umax_x2", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_umin(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_umin", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_umin_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_umin_x2", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_xor(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_xor", vdst, vaddr, vdata, saddr, modifier);}
    void global_atomic_xor_x2(const operand_t & vdst, const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_atomic_xor_x2", vdst, vaddr, vdata, saddr, modifier);}
    void global_load_dword(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_load_dword", vdst, vaddr, saddr, operand_t(), modifier);}
    void global_load_dword_addtid(const operand_t & vdst, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_load_dword_addtid", vdst, saddr, operand_t(), operand_t(), modifier);}
    void global_load_dwordx2(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_load_dwordx2", vdst, vaddr, saddr, operand_t(), modifier);}
    void global_load_dwordx3(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_load_dwordx3", vdst, vaddr, saddr, operand_t(), modifier);}
    void global_load_dwordx4(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_load_dwordx4", vdst, vaddr, saddr, operand_t(), modifier);}
    void global_load_sbyte(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_load_sbyte", vdst, vaddr, saddr, operand_t(), modifier);}
    void global_load_sbyte_d16(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_load_sbyte_d16", vdst, vaddr, saddr, operand_t(), modifier);}
    void global_load_sbyte_d16_hi(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_load_sbyte_d16_hi", vdst, vaddr, saddr, operand_t(), modifier);}
    void global_load_short_d16(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_load_short_d16", vdst, vaddr, saddr, operand_t(), modifier);}
    void global_load_short_d16_hi(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_load_short_d16_hi", vdst, vaddr, saddr, operand_t(), modifier);}
    void global_load_sshort(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_load_sshort", vdst, vaddr, saddr, operand_t(), modifier);}
    void global_load_ubyte(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_load_ubyte", vdst, vaddr, saddr, operand_t(), modifier);}
    void global_load_ubyte_d16(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_load_ubyte_d16", vdst, vaddr, saddr, operand_t(), modifier);}
    void global_load_ubyte_d16_hi(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_load_ubyte_d16_hi", vdst, vaddr, saddr, operand_t(), modifier);}
    void global_load_ushort(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_load_ushort", vdst, vaddr, saddr, operand_t(), modifier);}
    void global_store_byte(const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_store_byte", operand_t(), vaddr, vdata, saddr, modifier);}
    void global_store_byte_d16_hi(const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_store_byte_d16_hi", operand_t(), vaddr, vdata, saddr, modifier);}
    void global_store_dword(const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_store_dword", operand_t(), vaddr, vdata, saddr, modifier);}
    void global_store_dword_addtid(const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_store_dword_addtid", operand_t(), vdata, saddr, operand_t(), modifier);}
    void global_store_dwordx2(const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_store_dwordx2", operand_t(), vaddr, vdata, saddr, modifier);}
    void global_store_dwordx3(const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_store_dwordx3", operand_t(), vaddr, vdata, saddr, modifier);}
    void global_store_dwordx4(const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_store_dwordx4", operand_t(), vaddr, vdata, saddr, modifier);}
    void global_store_short(const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_store_short", operand_t(), vaddr, vdata, saddr, modifier);}
    void global_store_short_d16_hi(const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("global_store_short_d16_hi", operand_t(), vaddr, vdata, saddr, modifier);}
    void scratch_load_dword(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_load_dword", vdst, vaddr, saddr, operand_t(), modifier);}
    void scratch_load_dwordx2(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_load_dwordx2", vdst, vaddr, saddr, operand_t(), modifier);}
    void scratch_load_dwordx3(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_load_dwordx3", vdst, vaddr, saddr, operand_t(), modifier);}
    void scratch_load_dwordx4(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_load_dwordx4", vdst, vaddr, saddr, operand_t(), modifier);}
    void scratch_load_sbyte(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_load_sbyte", vdst, vaddr, saddr, operand_t(), modifier);}
    void scratch_load_sbyte_d16(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_load_sbyte_d16", vdst, vaddr, saddr, operand_t(), modifier);}
    void scratch_load_sbyte_d16_hi(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_load_sbyte_d16_hi", vdst, vaddr, saddr, operand_t(), modifier);}
    void scratch_load_short_d16(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_load_short_d16", vdst, vaddr, saddr, operand_t(), modifier);}
    void scratch_load_short_d16_hi(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_load_short_d16_hi", vdst, vaddr, saddr, operand_t(), modifier);}
    void scratch_load_sshort(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_load_sshort", vdst, vaddr, saddr, operand_t(), modifier);}
    void scratch_load_ubyte(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_load_ubyte", vdst, vaddr, saddr, operand_t(), modifier);}
    void scratch_load_ubyte_d16(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_load_ubyte_d16", vdst, vaddr, saddr, operand_t(), modifier);}
    void scratch_load_ubyte_d16_hi(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_load_ubyte_d16_hi", vdst, vaddr, saddr, operand_t(), modifier);}
    void scratch_load_ushort(const operand_t & vdst, const operand_t & vaddr, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_load_ushort", vdst, vaddr, saddr, operand_t(), modifier);}
    void scratch_store_byte(const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_store_byte", operand_t(), vaddr, vdata, saddr, modifier);}
    void scratch_store_byte_d16_hi(const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_store_byte_d16_hi", operand_t(), vaddr, vdata, saddr, modifier);}
    void scratch_store_dword(const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_store_dword", operand_t(), vaddr, vdata, saddr, modifier);}
    void scratch_store_dwordx2(const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_store_dwordx2", operand_t(), vaddr, vdata, saddr, modifier);}
    void scratch_store_dwordx3(const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_store_dwordx3", operand_t(), vaddr, vdata, saddr, modifier);}
    void scratch_store_dwordx4(const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_store_dwordx4", operand_t(), vaddr, vdata, saddr, modifier);}
    void scratch_store_short(const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_store_short", operand_t(), vaddr, vdata, saddr, modifier);}
    void scratch_store_short_d16_hi(const operand_t & vaddr, const operand_t & vdata, const operand_t & saddr, const std::initializer_list<modifier_t> & modifier = {}) {r_flat("scratch_store_short_d16_hi", operand_t(), vaddr, vdata, saddr, modifier);}
    
    
    // MUBUF
    void buffer_atomic_add(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_add", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_add_x2(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_add_x2", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_and(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_and", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_and_x2(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_and_x2", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_cmpswap(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_cmpswap", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_cmpswap_x2(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_cmpswap_x2", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_csub(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_csub", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_dec(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_dec", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_dec_x2(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_dec_x2", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_fcmpswap(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_fcmpswap", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_fcmpswap_x2(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_fcmpswap_x2", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_fmax(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_fmax", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_fmax_x2(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_fmax_x2", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_fmin(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_fmin", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_fmin_x2(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_fmin_x2", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_inc(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_inc", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_inc_x2(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_inc_x2", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_or(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_or", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_or_x2(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_or_x2", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_smax(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_smax", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_smax_x2(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_smax_x2", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_smin(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_smin", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_smin_x2(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_smin_x2", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_sub(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_sub", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_sub_x2(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_sub_x2", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_swap(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_swap", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_swap_x2(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_swap_x2", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_umax(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_umax", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_umax_x2(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_umax_x2", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_umin(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_umin", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_umin_x2(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_umin_x2", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_xor(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_xor", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_atomic_xor_x2(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_atomic_xor_x2", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_gl0_inv() {r_mubuf("buffer_gl0_inv", operand_t(), operand_t(), operand_t(), operand_t(), operand_t());}
    void buffer_gl1_inv() {r_mubuf("buffer_gl1_inv", operand_t(), operand_t(), operand_t(), operand_t(), operand_t());}
    void buffer_load_dword(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_dword", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_dwordx2(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_dwordx2", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_dwordx3(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_dwordx3", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_dwordx4(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_dwordx4", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_format_d16_x(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_format_d16_x", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_format_d16_xy(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_format_d16_xy", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_format_d16_xyz(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_format_d16_xyz", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_format_d16_xyzw(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_format_d16_xyzw", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_format_x(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_format_x", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_format_xy(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_format_xy", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_format_xyz(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_format_xyz", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_format_xyzw(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_format_xyzw", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_sbyte(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_sbyte", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_sbyte_d16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_sbyte_d16", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_sbyte_d16_hi(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_sbyte_d16_hi", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_short_d16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_short_d16", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_short_d16_hi(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_short_d16_hi", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_sshort(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_sshort", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_ubyte(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_ubyte", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_ubyte_d16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_ubyte_d16", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_ubyte_d16_hi(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_ubyte_d16_hi", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_load_ushort(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_load_ushort", vdst, src0, src1, src2, operand_t(), modifier);}
    void buffer_store_byte(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_store_byte", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_store_byte_d16_hi(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_store_byte_d16_hi", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_store_dword(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_store_dword", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_store_dwordx2(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_store_dwordx2", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_store_dwordx3(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_store_dwordx3", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_store_dwordx4(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_store_dwordx4", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_store_format_d16_x(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_store_format_d16_x", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_store_format_d16_xy(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_store_format_d16_xy", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_store_format_d16_xyz(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_store_format_d16_xyz", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_store_format_d16_xyzw(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_store_format_d16_xyzw", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_store_format_x(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_store_format_x", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_store_format_xy(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_store_format_xy", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_store_format_xyz(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_store_format_xyz", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_store_format_xyzw(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_store_format_xyzw", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_store_short(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_store_short", operand_t(), src0, src1, src2, src3, modifier);}
    void buffer_store_short_d16_hi(const operand_t & src0, const operand_t & src1, const operand_t & src2, const operand_t & src3, const std::initializer_list<modifier_t> & modifier = {}) {r_mubuf("buffer_store_short_d16_hi", operand_t(), src0, src1, src2, src3, modifier);}


    // SMEM
    void s_buffer_load_dword(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1, const std::initializer_list<modifier_t> & modifier = {}) {r_smem("s_buffer_load_dword", sdst, ssrc0, ssrc1, operand_t(), modifier);}
    void s_buffer_load_dwordx16(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1, const std::initializer_list<modifier_t> & modifier = {}) {r_smem("s_buffer_load_dwordx16", sdst, ssrc0, ssrc1, operand_t(), modifier);}
    void s_buffer_load_dwordx2(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1, const std::initializer_list<modifier_t> & modifier = {}) {r_smem("s_buffer_load_dwordx2", sdst, ssrc0, ssrc1, operand_t(), modifier);}
    void s_buffer_load_dwordx4(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1, const std::initializer_list<modifier_t> & modifier = {}) {r_smem("s_buffer_load_dwordx4", sdst, ssrc0, ssrc1, operand_t(), modifier);}
    void s_buffer_load_dwordx8(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1, const std::initializer_list<modifier_t> & modifier = {}) {r_smem("s_buffer_load_dwordx8", sdst, ssrc0, ssrc1, operand_t(), modifier);}
    void s_dcache_inv() {r_smem("s_dcache_inv", operand_t(), operand_t(), operand_t(), operand_t());}
    void s_gl1_inv() {r_smem("s_gl1_inv", operand_t(), operand_t(), operand_t(), operand_t());}
    void s_load_dword(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1, const std::initializer_list<modifier_t> & modifier = {}) {r_smem("s_load_dword", sdst, ssrc0, ssrc1, operand_t(), modifier);}
    void s_load_dwordx16(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1, const std::initializer_list<modifier_t> & modifier = {}) {r_smem("s_load_dwordx16", sdst, ssrc0, ssrc1, operand_t(), modifier);}
    void s_load_dwordx2(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1, const std::initializer_list<modifier_t> & modifier = {}) {r_smem("s_load_dwordx2", sdst, ssrc0, ssrc1, operand_t(), modifier);}
    void s_load_dwordx4(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1, const std::initializer_list<modifier_t> & modifier = {}) {r_smem("s_load_dwordx4", sdst, ssrc0, ssrc1, operand_t(), modifier);}
    void s_load_dwordx8(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1, const std::initializer_list<modifier_t> & modifier = {}) {r_smem("s_load_dwordx8", sdst, ssrc0, ssrc1, operand_t(), modifier);}
    void s_memrealtime(const operand_t & sdst) {r_smem("s_memrealtime", sdst, operand_t(), operand_t(), operand_t());}
    void s_memtime(const operand_t & sdst) {r_smem("s_memtime", sdst, operand_t(), operand_t(), operand_t());}


    // SOP1
    void s_abs_i32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_abs_i32", sdst, ssrc);}
    void s_and_saveexec_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_and_saveexec_b32", sdst, ssrc);}
    void s_and_saveexec_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_and_saveexec_b64", sdst, ssrc);}
    void s_andn1_saveexec_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_andn1_saveexec_b32", sdst, ssrc);}
    void s_andn1_saveexec_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_andn1_saveexec_b64", sdst, ssrc);}
    void s_andn1_wrexec_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_andn1_wrexec_b32", sdst, ssrc);}
    void s_andn1_wrexec_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_andn1_wrexec_b64", sdst, ssrc);}
    void s_andn2_saveexec_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_andn2_saveexec_b32", sdst, ssrc);}
    void s_andn2_saveexec_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_andn2_saveexec_b64", sdst, ssrc);}
    void s_andn2_wrexec_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_andn2_wrexec_b32", sdst, ssrc);}
    void s_andn2_wrexec_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_andn2_wrexec_b64", sdst, ssrc);}
    void s_bcnt0_i32_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_bcnt0_i32_b32", sdst, ssrc);}
    void s_bcnt0_i32_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_bcnt0_i32_b64", sdst, ssrc);}
    void s_bcnt1_i32_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_bcnt1_i32_b32", sdst, ssrc);}
    void s_bcnt1_i32_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_bcnt1_i32_b64", sdst, ssrc);}
    void s_bitreplicate_b64_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_bitreplicate_b64_b32", sdst, ssrc);}
    void s_bitset0_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_bitset0_b32", sdst, ssrc);}
    void s_bitset0_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_bitset0_b64", sdst, ssrc);}
    void s_bitset1_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_bitset1_b32", sdst, ssrc);}
    void s_bitset1_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_bitset1_b64", sdst, ssrc);}
    void s_brev_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_brev_b32", sdst, ssrc);}
    void s_brev_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_brev_b64", sdst, ssrc);}
    void s_cmov_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_cmov_b32", sdst, ssrc);}
    void s_cmov_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_cmov_b64", sdst, ssrc);}
    void s_ff0_i32_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_ff0_i32_b32", sdst, ssrc);}
    void s_ff0_i32_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_ff0_i32_b64", sdst, ssrc);}
    void s_ff1_i32_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_ff1_i32_b32", sdst, ssrc);}
    void s_ff1_i32_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_ff1_i32_b64", sdst, ssrc);}
    void s_flbit_i32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_flbit_i32", sdst, ssrc);}
    void s_flbit_i32_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_flbit_i32_b32", sdst, ssrc);}
    void s_flbit_i32_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_flbit_i32_b64", sdst, ssrc);}
    void s_flbit_i32_i64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_flbit_i32_i64", sdst, ssrc);}
    void s_getpc_b64(const operand_t & sdst) {r_sop1("s_getpc_b64", sdst, operand_t());}
    void s_mov_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_mov_b32", sdst, ssrc);}
    void s_mov_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_mov_b64", sdst, ssrc);}
    void s_movreld_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_movreld_b32", sdst, ssrc);}
    void s_movreld_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_movreld_b64", sdst, ssrc);}
    void s_movrels_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_movrels_b32", sdst, ssrc);}
    void s_movrels_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_movrels_b64", sdst, ssrc);}
    void s_movrelsd_2_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_movrelsd_2_b32", sdst, ssrc);}
    void s_nand_saveexec_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_nand_saveexec_b32", sdst, ssrc);}
    void s_nand_saveexec_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_nand_saveexec_b64", sdst, ssrc);}
    void s_nor_saveexec_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_nor_saveexec_b32", sdst, ssrc);}
    void s_nor_saveexec_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_nor_saveexec_b64", sdst, ssrc);}
    void s_not_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_not_b32", sdst, ssrc);}
    void s_not_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_not_b64", sdst, ssrc);}
    void s_or_saveexec_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_or_saveexec_b32", sdst, ssrc);}
    void s_or_saveexec_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_or_saveexec_b64", sdst, ssrc);}
    void s_orn1_saveexec_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_orn1_saveexec_b32", sdst, ssrc);}
    void s_orn1_saveexec_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_orn1_saveexec_b64", sdst, ssrc);}
    void s_orn2_saveexec_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_orn2_saveexec_b32", sdst, ssrc);}
    void s_orn2_saveexec_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_orn2_saveexec_b64", sdst, ssrc);}
    void s_quadmask_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_quadmask_b32", sdst, ssrc);}
    void s_quadmask_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_quadmask_b64", sdst, ssrc);}
    void s_rfe_b64(const operand_t & ssrc) {r_sop1("s_rfe_b64", operand_t(), ssrc);}
    void s_setpc_b64(const operand_t & ssrc) {r_sop1("s_setpc_b64", operand_t(), ssrc);}
    void s_sext_i32_i16(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_sext_i32_i16", sdst, ssrc);}
    void s_sext_i32_i8(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_sext_i32_i8", sdst, ssrc);}
    void s_swappc_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_swappc_b64", sdst, ssrc);}
    void s_wqm_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_wqm_b32", sdst, ssrc);}
    void s_wqm_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_wqm_b64", sdst, ssrc);}
    void s_xnor_saveexec_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_xnor_saveexec_b32", sdst, ssrc);}
    void s_xnor_saveexec_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_xnor_saveexec_b64", sdst, ssrc);}
    void s_xor_saveexec_b32(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_xor_saveexec_b32", sdst, ssrc);}
    void s_xor_saveexec_b64(const operand_t & sdst, const operand_t & ssrc) {r_sop1("s_xor_saveexec_b64", sdst, ssrc);}


    // SOP2
    void s_absdiff_i32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_absdiff_i32", sdst, ssrc0, ssrc1);}
    void s_add_i32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_add_i32", sdst, ssrc0, ssrc1);}
    void s_add_u32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_add_u32", sdst, ssrc0, ssrc1);}
    void s_addc_u32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_addc_u32", sdst, ssrc0, ssrc1);}
    void s_and_b32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_and_b32", sdst, ssrc0, ssrc1);}
    void s_and_b64(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_and_b64", sdst, ssrc0, ssrc1);}
    void s_andn2_b32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_andn2_b32", sdst, ssrc0, ssrc1);}
    void s_andn2_b64(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_andn2_b64", sdst, ssrc0, ssrc1);}
    void s_ashr_i32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_ashr_i32", sdst, ssrc0, ssrc1);}
    void s_ashr_i64(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_ashr_i64", sdst, ssrc0, ssrc1);}
    void s_bfe_i32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_bfe_i32", sdst, ssrc0, ssrc1);}
    void s_bfe_i64(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_bfe_i64", sdst, ssrc0, ssrc1);}
    void s_bfe_u32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_bfe_u32", sdst, ssrc0, ssrc1);}
    void s_bfe_u64(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_bfe_u64", sdst, ssrc0, ssrc1);}
    void s_bfm_b32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_bfm_b32", sdst, ssrc0, ssrc1);}
    void s_bfm_b64(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_bfm_b64", sdst, ssrc0, ssrc1);}
    void s_cselect_b32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_cselect_b32", sdst, ssrc0, ssrc1);}
    void s_cselect_b64(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_cselect_b64", sdst, ssrc0, ssrc1);}
    void s_lshl1_add_u32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_lshl1_add_u32", sdst, ssrc0, ssrc1);}
    void s_lshl2_add_u32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_lshl2_add_u32", sdst, ssrc0, ssrc1);}
    void s_lshl3_add_u32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_lshl3_add_u32", sdst, ssrc0, ssrc1);}
    void s_lshl4_add_u32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_lshl4_add_u32", sdst, ssrc0, ssrc1);}
    void s_lshl_b32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_lshl_b32", sdst, ssrc0, ssrc1);}
    void s_lshl_b64(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_lshl_b64", sdst, ssrc0, ssrc1);}
    void s_lshr_b32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_lshr_b32", sdst, ssrc0, ssrc1);}
    void s_lshr_b64(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_lshr_b64", sdst, ssrc0, ssrc1);}
    void s_max_i32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_max_i32", sdst, ssrc0, ssrc1);}
    void s_max_u32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_max_u32", sdst, ssrc0, ssrc1);}
    void s_min_i32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_min_i32", sdst, ssrc0, ssrc1);}
    void s_min_u32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_min_u32", sdst, ssrc0, ssrc1);}
    void s_mul_hi_i32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_mul_hi_i32", sdst, ssrc0, ssrc1);}
    void s_mul_hi_u32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_mul_hi_u32", sdst, ssrc0, ssrc1);}
    void s_mul_i32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_mul_i32", sdst, ssrc0, ssrc1);}
    void s_nand_b32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_nand_b32", sdst, ssrc0, ssrc1);}
    void s_nand_b64(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_nand_b64", sdst, ssrc0, ssrc1);}
    void s_nor_b32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_nor_b32", sdst, ssrc0, ssrc1);}
    void s_nor_b64(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_nor_b64", sdst, ssrc0, ssrc1);}
    void s_or_b32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_or_b32", sdst, ssrc0, ssrc1);}
    void s_or_b64(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_or_b64", sdst, ssrc0, ssrc1);}
    void s_orn2_b32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_orn2_b32", sdst, ssrc0, ssrc1);}
    void s_orn2_b64(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_orn2_b64", sdst, ssrc0, ssrc1);}
    void s_pack_hh_b32_b16(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_pack_hh_b32_b16", sdst, ssrc0, ssrc1);}
    void s_pack_lh_b32_b16(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_pack_lh_b32_b16", sdst, ssrc0, ssrc1);}
    void s_pack_ll_b32_b16(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_pack_ll_b32_b16", sdst, ssrc0, ssrc1);}
    void s_sub_i32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_sub_i32", sdst, ssrc0, ssrc1);}
    void s_sub_u32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_sub_u32", sdst, ssrc0, ssrc1);}
    void s_subb_u32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_subb_u32", sdst, ssrc0, ssrc1);}
    void s_xnor_b32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_xnor_b32", sdst, ssrc0, ssrc1);}
    void s_xnor_b64(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_xnor_b64", sdst, ssrc0, ssrc1);}
    void s_xor_b32(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_xor_b32", sdst, ssrc0, ssrc1);}
    void s_xor_b64(const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) {r_sop2("s_xor_b64", sdst, ssrc0, ssrc1);}


    // SOPC
    void s_bitcmp0_b32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopc("s_bitcmp0_b32", ssrc0, ssrc1);}
    void s_bitcmp0_b64(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopc("s_bitcmp0_b64", ssrc0, ssrc1);}
    void s_bitcmp1_b32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopc("s_bitcmp1_b32", ssrc0, ssrc1);}
    void s_bitcmp1_b64(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopc("s_bitcmp1_b64", ssrc0, ssrc1);}
    void s_cmp_eq_i32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopc("s_cmp_eq_i32", ssrc0, ssrc1);}
    void s_cmp_eq_u32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopc("s_cmp_eq_u32", ssrc0, ssrc1);}
    void s_cmp_eq_u64(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopc("s_cmp_eq_u64", ssrc0, ssrc1);}
    void s_cmp_ge_i32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopc("s_cmp_ge_i32", ssrc0, ssrc1);}
    void s_cmp_ge_u32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopc("s_cmp_ge_u32", ssrc0, ssrc1);}
    void s_cmp_gt_i32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopc("s_cmp_gt_i32", ssrc0, ssrc1);}
    void s_cmp_gt_u32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopc("s_cmp_gt_u32", ssrc0, ssrc1);}
    void s_cmp_le_i32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopc("s_cmp_le_i32", ssrc0, ssrc1);}
    void s_cmp_le_u32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopc("s_cmp_le_u32", ssrc0, ssrc1);}
    void s_cmp_lg_i32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopc("s_cmp_lg_i32", ssrc0, ssrc1);}
    void s_cmp_lg_u32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopc("s_cmp_lg_u32", ssrc0, ssrc1);}
    void s_cmp_lg_u64(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopc("s_cmp_lg_u64", ssrc0, ssrc1);}
    void s_cmp_lt_i32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopc("s_cmp_lt_i32", ssrc0, ssrc1);}
    void s_cmp_lt_u32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopc("s_cmp_lt_u32", ssrc0, ssrc1);}


    // SOPK
    void s_addk_i32(const operand_t & sdst, const operand_t & ssrc) {r_sopk("s_addk_i32", sdst, ssrc, operand_t());}
    void s_call_b64(const operand_t & sdst, const operand_t & ssrc) {r_sopk("s_call_b64", sdst, ssrc, operand_t());}
    void s_cmovk_i32(const operand_t & sdst, const operand_t & ssrc) {r_sopk("s_cmovk_i32", sdst, ssrc, operand_t());}
    void s_cmpk_eq_i32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopk("s_cmpk_eq_i32", operand_t(), ssrc0, ssrc1);}
    void s_cmpk_eq_u32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopk("s_cmpk_eq_u32", operand_t(), ssrc0, ssrc1);}
    void s_cmpk_ge_i32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopk("s_cmpk_ge_i32", operand_t(), ssrc0, ssrc1);}
    void s_cmpk_ge_u32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopk("s_cmpk_ge_u32", operand_t(), ssrc0, ssrc1);}
    void s_cmpk_gt_i32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopk("s_cmpk_gt_i32", operand_t(), ssrc0, ssrc1);}
    void s_cmpk_gt_u32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopk("s_cmpk_gt_u32", operand_t(), ssrc0, ssrc1);}
    void s_cmpk_le_i32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopk("s_cmpk_le_i32", operand_t(), ssrc0, ssrc1);}
    void s_cmpk_le_u32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopk("s_cmpk_le_u32", operand_t(), ssrc0, ssrc1);}
    void s_cmpk_lg_i32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopk("s_cmpk_lg_i32", operand_t(), ssrc0, ssrc1);}
    void s_cmpk_lg_u32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopk("s_cmpk_lg_u32", operand_t(), ssrc0, ssrc1);}
    void s_cmpk_lt_i32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopk("s_cmpk_lt_i32", operand_t(), ssrc0, ssrc1);}
    void s_cmpk_lt_u32(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopk("s_cmpk_lt_u32", operand_t(), ssrc0, ssrc1);}
    void s_getreg_b32(const operand_t & sdst, const operand_t & ssrc) {r_sopk("s_getreg_b32", sdst, ssrc, operand_t());}
    void s_movk_i32(const operand_t & sdst, const operand_t & ssrc) {r_sopk("s_movk_i32", sdst, ssrc, operand_t());}
    void s_mulk_i32(const operand_t & sdst, const operand_t & ssrc) {r_sopk("s_mulk_i32", sdst, ssrc, operand_t());}
    void s_setreg_b32(const operand_t & sdst, const operand_t & ssrc) {r_sopk("s_setreg_b32", sdst, ssrc, operand_t());}
    void s_setreg_imm32_b32(const operand_t & sdst, const operand_t & ssrc) {r_sopk("s_setreg_imm32_b32", sdst, ssrc, operand_t());}
    void s_subvector_loop_begin(const operand_t & sdst, const operand_t & ssrc) {r_sopk("s_subvector_loop_begin", sdst, ssrc, operand_t());}
    void s_subvector_loop_end(const operand_t & sdst, const operand_t & ssrc) {r_sopk("s_subvector_loop_end", sdst, ssrc, operand_t());}
    void s_version(const operand_t & ssrc) {r_sopk("s_version", operand_t(), ssrc, operand_t());}
    void s_waitcnt_expcnt(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopk("s_waitcnt_expcnt", operand_t(), ssrc0, ssrc1);}
    void s_waitcnt_lgkmcnt(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopk("s_waitcnt_lgkmcnt", operand_t(), ssrc0, ssrc1);}
    void s_waitcnt_vmcnt(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopk("s_waitcnt_vmcnt", operand_t(), ssrc0, ssrc1);}
    void s_waitcnt_vscnt(const operand_t & ssrc0, const operand_t & ssrc1) {r_sopk("s_waitcnt_vscnt", operand_t(), ssrc0, ssrc1);}


    // SOPP
    void s_barrier() {r_sopp("s_barrier", operand_t());}
    void s_branch(const operand_t & src) {r_sopp("s_branch", src);}
    void s_cbranch_cdbgsys(const operand_t & src) {r_sopp("s_cbranch_cdbgsys", src);}
    void s_cbranch_cdbgsys_and_user(const operand_t & src) {r_sopp("s_cbranch_cdbgsys_and_user", src);}
    void s_cbranch_cdbgsys_or_user(const operand_t & src) {r_sopp("s_cbranch_cdbgsys_or_user", src);}
    void s_cbranch_cdbguser(const operand_t & src) {r_sopp("s_cbranch_cdbguser", src);}
    void s_cbranch_execnz(const operand_t & src) {r_sopp("s_cbranch_execnz", src);}
    void s_cbranch_execz(const operand_t & src) {r_sopp("s_cbranch_execz", src);}
    void s_cbranch_scc0(const operand_t & src) {r_sopp("s_cbranch_scc0", src);}
    void s_cbranch_scc1(const operand_t & src) {r_sopp("s_cbranch_scc1", src);}
    void s_cbranch_vccnz(const operand_t & src) {r_sopp("s_cbranch_vccnz", src);}
    void s_cbranch_vccz(const operand_t & src) {r_sopp("s_cbranch_vccz", src);}
    void s_clause(const operand_t & src) {r_sopp("s_clause", src);}
    void s_code_end() {r_sopp("s_code_end", operand_t());}
    void s_decperflevel(const operand_t & src) {r_sopp("s_decperflevel", src);}
    void s_denorm_mode(const operand_t & src) {r_sopp("s_denorm_mode", src);}
    void s_endpgm() {r_sopp("s_endpgm", operand_t());}
    void s_endpgm_ordered_ps_done() {r_sopp("s_endpgm_ordered_ps_done", operand_t());}
    void s_endpgm_saved() {r_sopp("s_endpgm_saved", operand_t());}
    void s_icache_inv() {r_sopp("s_icache_inv", operand_t());}
    void s_incperflevel(const operand_t & src) {r_sopp("s_incperflevel", src);}
    void s_inst_prefetch(const operand_t & src) {r_sopp("s_inst_prefetch", src);}
    void s_nop(const operand_t & src) {r_sopp("s_nop", src);}
    void s_round_mode(const operand_t & src) {r_sopp("s_round_mode", src);}
    void s_sendmsg(const operand_t & src) {r_sopp("s_sendmsg", src);}
    void s_sendmsghalt(const operand_t & src) {r_sopp("s_sendmsghalt", src);}
    void s_sethalt(const operand_t & src) {r_sopp("s_sethalt", src);}
    void s_setkill(const operand_t & src) {r_sopp("s_setkill", src);}
    void s_setprio(const operand_t & src) {r_sopp("s_setprio", src);}
    void s_sleep(const operand_t & src) {r_sopp("s_sleep", src);}
    void s_trap(const operand_t & src) {r_sopp("s_trap", src);}
    void s_ttracedata() {r_sopp("s_ttracedata", operand_t());}
    void s_ttracedata_imm(const operand_t & src) {r_sopp("s_ttracedata_imm", src);}
    void s_waitcnt(const operand_t & src) {r_sopp("s_waitcnt", src);}
    void s_waitcnt_depctr(const operand_t & src) {r_sopp("s_waitcnt_depctr", src);}
    void s_wakeup() {r_sopp("s_wakeup", operand_t());}


    // VOP1
    void v_bfrev_b32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_bfrev_b32", vdst, src, modifier); }
    void v_ceil_f16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_ceil_f16", vdst, src, modifier); }
    void v_ceil_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_ceil_f32", vdst, src, modifier); }
    void v_ceil_f64(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_ceil_f64", vdst, src, modifier); }
    void v_clrexcp() {r_vop1("v_clrexcp", operand_t(), operand_t());}
    void v_cos_f16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cos_f16", vdst, src, modifier); }
    void v_cos_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cos_f32", vdst, src, modifier); }
    void v_cvt_f16_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_f16_f32", vdst, src, modifier); }
    void v_cvt_f16_i16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_f16_i16", vdst, src, modifier); }
    void v_cvt_f16_u16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_f16_u16", vdst, src, modifier); }
    void v_cvt_f32_f16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_f32_f16", vdst, src, modifier); }
    void v_cvt_f32_f64(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_f32_f64", vdst, src, modifier); }
    void v_cvt_f32_i32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_f32_i32", vdst, src, modifier); }
    void v_cvt_f32_u32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_f32_u32", vdst, src, modifier); }
    void v_cvt_f32_ubyte0(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_f32_ubyte0", vdst, src, modifier); }
    void v_cvt_f32_ubyte1(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_f32_ubyte1", vdst, src, modifier); }
    void v_cvt_f32_ubyte2(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_f32_ubyte2", vdst, src, modifier); }
    void v_cvt_f32_ubyte3(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_f32_ubyte3", vdst, src, modifier); }
    void v_cvt_f64_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_f64_f32", vdst, src, modifier); }
    void v_cvt_f64_i32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_f64_i32", vdst, src, modifier); }
    void v_cvt_f64_u32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_f64_u32", vdst, src, modifier); }
    void v_cvt_flr_i32_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_flr_i32_f32", vdst, src, modifier); }
    void v_cvt_i16_f16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_i16_f16", vdst, src, modifier); }
    void v_cvt_i32_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_i32_f32", vdst, src, modifier); }
    void v_cvt_i32_f64(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_i32_f64", vdst, src, modifier); }
    void v_cvt_norm_i16_f16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_norm_i16_f16", vdst, src, modifier); }
    void v_cvt_norm_u16_f16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_norm_u16_f16", vdst, src, modifier); }
    void v_cvt_off_f32_i4(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_off_f32_i4", vdst, src, modifier); }
    void v_cvt_rpi_i32_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_rpi_i32_f32", vdst, src, modifier); }
    void v_cvt_u16_f16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_u16_f16", vdst, src, modifier); }
    void v_cvt_u32_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_u32_f32", vdst, src, modifier); }
    void v_cvt_u32_f64(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_cvt_u32_f64", vdst, src, modifier); }
    void v_exp_f16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_exp_f16", vdst, src, modifier); }
    void v_exp_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_exp_f32", vdst, src, modifier); }
    void v_ffbh_i32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_ffbh_i32", vdst, src, modifier); }
    void v_ffbh_u32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_ffbh_u32", vdst, src, modifier); }
    void v_ffbl_b32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_ffbl_b32", vdst, src, modifier); }
    void v_floor_f16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_floor_f16", vdst, src, modifier); }
    void v_floor_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_floor_f32", vdst, src, modifier); }
    void v_floor_f64(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_floor_f64", vdst, src, modifier); }
    void v_fract_f16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_fract_f16", vdst, src, modifier); }
    void v_fract_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_fract_f32", vdst, src, modifier); }
    void v_fract_f64(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_fract_f64", vdst, src, modifier); }
    void v_frexp_exp_i16_f16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_frexp_exp_i16_f16", vdst, src, modifier); }
    void v_frexp_exp_i32_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_frexp_exp_i32_f32", vdst, src, modifier); }
    void v_frexp_exp_i32_f64(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_frexp_exp_i32_f64", vdst, src, modifier); }
    void v_frexp_mant_f16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_frexp_mant_f16", vdst, src, modifier); }
    void v_frexp_mant_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_frexp_mant_f32", vdst, src, modifier); }
    void v_frexp_mant_f64(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_frexp_mant_f64", vdst, src, modifier); }
    void v_log_f16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_log_f16", vdst, src, modifier); }
    void v_log_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_log_f32", vdst, src, modifier); }
    void v_mov_b32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_mov_b32", vdst, src, modifier); }
    void v_movreld_b32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_movreld_b32", vdst, src, modifier); }
    void v_movrels_b32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_movrels_b32", vdst, src, modifier); }
    void v_movrelsd_2_b32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_movrelsd_2_b32", vdst, src, modifier); }
    void v_movrelsd_b32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_movrelsd_b32", vdst, src, modifier); }
    void v_nop() {r_vop1("v_nop", operand_t(), operand_t());}
    void v_not_b32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_not_b32", vdst, src, modifier); }
    void v_pipeflush() {r_vop1("v_pipeflush", operand_t(), operand_t());}
    void v_rcp_f16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_rcp_f16", vdst, src, modifier); }
    void v_rcp_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_rcp_f32", vdst, src, modifier); }
    void v_rcp_f64(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_rcp_f64", vdst, src, modifier); }
    void v_rcp_iflag_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_rcp_iflag_f32", vdst, src, modifier); }
    void v_readfirstlane_b32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_readfirstlane_b32", vdst, src, modifier); }
    void v_rndne_f16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_rndne_f16", vdst, src, modifier); }
    void v_rndne_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_rndne_f32", vdst, src, modifier); }
    void v_rndne_f64(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_rndne_f64", vdst, src, modifier); }
    void v_rsq_f16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_rsq_f16", vdst, src, modifier); }
    void v_rsq_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_rsq_f32", vdst, src, modifier); }
    void v_rsq_f64(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_rsq_f64", vdst, src, modifier); }
    void v_sat_pk_u8_i16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_sat_pk_u8_i16", vdst, src, modifier); }
    void v_sin_f16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_sin_f16", vdst, src, modifier); }
    void v_sin_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_sin_f32", vdst, src, modifier); }
    void v_sqrt_f16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_sqrt_f16", vdst, src, modifier); }
    void v_sqrt_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_sqrt_f32", vdst, src, modifier); }
    void v_sqrt_f64(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_sqrt_f64", vdst, src, modifier); }
    void v_swap_b32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_swap_b32", vdst, src, modifier); }
    void v_swaprel_b32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_swaprel_b32", vdst, src, modifier); }
    void v_trunc_f16(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_trunc_f16", vdst, src, modifier); }
    void v_trunc_f32(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_trunc_f32", vdst, src, modifier); }
    void v_trunc_f64(const operand_t & vdst, const operand_t & src, const std::initializer_list<modifier_t> & modifier = {}) { r_vop1("v_trunc_f64", vdst, src, modifier); }


    // VOP2 (->VOP3)
    void v_add_co_ci_u32(const operand_t & vdst, const operand_t & vcc_hi, const operand_t & src0, const operand_t & vsrc1, const operand_t & vcc_lo, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_add_co_ci_u32", vdst, vcc_hi, src0, vsrc1, vcc_lo, modifier); }
    void v_add_f16(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_add_f16", vdst, src0, vsrc1, modifier); }
    void v_add_f32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_add_f32", vdst, src0, vsrc1, modifier); }
    void v_add_nc_u32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2(ti.arch >= 1000 ? "v_add_nc_u32" : "v_add_u32", vdst, src0, vsrc1, modifier); }
    void v_addc_co_u32(const operand_t & vdst, const operand_t & vcc_hi, const operand_t & src0, const operand_t & vsrc1, const operand_t & vcc_lo, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_addc_co_u32", vdst, vcc_hi, src0, vsrc1, vcc_lo, modifier); }
    void v_and_b32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_and_b32", vdst, src0, vsrc1, modifier); }
    void v_ashrrev_i32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_ashrrev_i32", vdst, src0, vsrc1, modifier); }
    void v_cndmask_b32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const operand_t & vcc, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_cndmask_b32", vdst, src0, vsrc1, vcc, modifier); }
    void v_cvt_pkrtz_f16_f32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_cvt_pkrtz_f16_f32", vdst, src0, vsrc1, modifier); }
    void v_dot2c_f32_f16(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_dot2c_f32_f16", vdst, src0, vsrc1, modifier); }
    void v_dot4c_i32_i8(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_dot4c_i32_i8", vdst, src0, vsrc1, modifier); }
    void v_fmaak_f16(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const operand_t & simm32) { r_vop2("v_fmaak_f16", vdst, src0, vsrc1, simm32); }
    void v_fmaak_f32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const operand_t & simm32) { r_vop2("v_fmaak_f32", vdst, src0, vsrc1, simm32); }
    void v_fmac_f16(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_fmac_f16", vdst, src0, vsrc1, modifier); }
    void v_fmac_f32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_fmac_f32", vdst, src0, vsrc1, modifier); }
    void v_fmac_legacy_f32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1) { r_vop2("v_fmac_legacy_f32", vdst, src0, vsrc1); }
    void v_fmamk_f16(const operand_t & vdst, const operand_t & src0, const operand_t & simm32, const operand_t & vsrc2) { r_vop2(ti.arch >= 1000 ? "v_fmamk_f16" : "v_madmk_f16", vdst, src0, simm32, vsrc2); }
    void v_fmamk_f32(const operand_t & vdst, const operand_t & src0, const operand_t & simm32, const operand_t & vsrc2) { r_vop2(ti.arch >= 1000 ? "v_fmamk_f32" : "v_madmk_f32", vdst, src0, simm32, vsrc2); }
    void v_ldexp_f16(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_ldexp_f16", vdst, src0, vsrc1, modifier); }
    void v_lshlrev_b32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_lshlrev_b32", vdst, src0, vsrc1, modifier); }
    void v_lshrrev_b32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_lshrrev_b32", vdst, src0, vsrc1, modifier); }
    void v_max_f16(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_max_f16", vdst, src0, vsrc1, modifier); }
    void v_max_f32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_max_f32", vdst, src0, vsrc1, modifier); }
    void v_max_i32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_max_i32", vdst, src0, vsrc1, modifier); }
    void v_max_u32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_max_u32", vdst, src0, vsrc1, modifier); }
    void v_min_f16(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_min_f16", vdst, src0, vsrc1, modifier); }
    void v_min_f32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_min_f32", vdst, src0, vsrc1, modifier); }
    void v_min_i32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_min_i32", vdst, src0, vsrc1, modifier); }
    void v_min_u32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_min_u32", vdst, src0, vsrc1, modifier); }
    void v_mul_f16(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_mul_f16", vdst, src0, vsrc1, modifier); }
    void v_mul_f32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_mul_f32", vdst, src0, vsrc1, modifier); }
    void v_mul_hi_i32_i24(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_mul_hi_i32_i24", vdst, src0, vsrc1, modifier); }
    void v_mul_hi_u32_u24(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_mul_hi_u32_u24", vdst, src0, vsrc1, modifier); }
    void v_mul_i32_i24(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_mul_i32_i24", vdst, src0, vsrc1, modifier); }
    void v_mul_legacy_f32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_mul_legacy_f32", vdst, src0, vsrc1, modifier); }
    void v_mul_u32_u24(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_mul_u32_u24", vdst, src0, vsrc1, modifier); }
    void v_or_b32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_or_b32", vdst, src0, vsrc1, modifier); }
    void v_pk_fmac_f16(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_pk_fmac_f16", vdst, src0, vsrc1, modifier); }
    void v_sub_co_ci_u32(const operand_t & vdst, const operand_t & vcc_hi, const operand_t & src0, const operand_t & vsrc1, const operand_t & vcc_lo, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_sub_co_ci_u32", vdst, vcc_hi, src0, vsrc1, vcc_lo, modifier); }
    void v_sub_f16(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_sub_f16", vdst, src0, vsrc1, modifier); }
    void v_sub_f32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_sub_f32", vdst, src0, vsrc1, modifier); }
    void v_sub_nc_u32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2(ti.arch >= 1000 ? "v_sub_nc_u32" : "v_sub_u32", vdst, src0, vsrc1, modifier); }
    void v_subrev_co_ci_u32(const operand_t & vdst, const operand_t & vcc_hi, const operand_t & src0, const operand_t & vsrc1, const operand_t & vcc_lo, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_subrev_co_ci_u32", vdst, vcc_hi, src0, vsrc1, vcc_lo, modifier); }
    void v_subrev_f16(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_subrev_f16", vdst, src0, vsrc1, modifier); }
    void v_subrev_f32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_subrev_f32", vdst, src0, vsrc1, modifier); }
    void v_subrev_nc_u32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2(ti.arch >= 1000 ? "v_subrev_nc_u32" : "v_subrev_u32", vdst, src0, vsrc1, modifier); }
    void v_xnor_b32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_xnor_b32", vdst, src0, vsrc1, modifier); }
    void v_xor_b32(const operand_t & vdst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2("v_xor_b32", vdst, src0, vsrc1, modifier); }


    // VOP3
    void v_add3_u32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_add3_u32", vdst, src0, src1, src2); }
    void v_add_co_u32(const operand_t & vdst, const operand_t & sdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_add_co_u32", vdst, sdst, src0, src1, operand_t(), modifier); }
    void v_add_f64(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_add_f64", vdst, src0, src1, modifier); }
    void v_add_lshl_u32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_add_lshl_u32", vdst, src0, src1, src2); }
    void v_add_nc_i16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_add_nc_i16", vdst, src0, src1, modifier); }
    void v_add_nc_i32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_add_nc_i32", vdst, src0, src1, modifier); }
    void v_add_nc_u16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_add_nc_u16", vdst, src0, src1, modifier); }
    void v_alignbit_b32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_alignbit_b32", vdst, src0, src1, src2); }
    void v_alignbyte_b32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_alignbyte_b32", vdst, src0, src1, src2); }
    void v_and_or_b32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_and_or_b32", vdst, src0, src1, src2); }
    void v_ashrrev_i16(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_ashrrev_i16", vdst, src0, src1); }
    void v_ashrrev_i64(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_ashrrev_i64", vdst, src0, src1); }
    void v_bcnt_u32_b32(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_bcnt_u32_b32", vdst, src0, src1); }
    void v_bfe_i32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_bfe_i32", vdst, src0, src1, src2); }
    void v_bfe_u32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_bfe_u32", vdst, src0, src1, src2); }
    void v_bfi_b32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_bfi_b32", vdst, src0, src1, src2); }
    void v_bfm_b32(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_bfm_b32", vdst, src0, src1); }
    void v_cubeid_f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_cubeid_f32", vdst, src0, src1, src2, modifier); }
    void v_cubema_f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_cubema_f32", vdst, src0, src1, src2, modifier); }
    void v_cubesc_f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_cubesc_f32", vdst, src0, src1, src2, modifier); }
    void v_cubetc_f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_cubetc_f32", vdst, src0, src1, src2, modifier); }
    void v_cvt_pk_i16_i32(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_cvt_pk_i16_i32", vdst, src0, src1); }
    void v_cvt_pk_u16_u32(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_cvt_pk_u16_u32", vdst, src0, src1); }
    void v_cvt_pk_u8_f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_cvt_pk_u8_f32", vdst, src0, src1, src2); }
    void v_cvt_pknorm_i16_f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_cvt_pknorm_i16_f16", vdst, src0, src1, modifier); }
    void v_cvt_pknorm_i16_f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_cvt_pknorm_i16_f32", vdst, src0, src1); }
    void v_cvt_pknorm_u16_f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_cvt_pknorm_u16_f16", vdst, src0, src1, modifier); }
    void v_cvt_pknorm_u16_f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_cvt_pknorm_u16_f32", vdst, src0, src1); }
    void v_div_fixup_f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_div_fixup_f16", vdst, src0, src1, src2, modifier); }
    void v_div_fixup_f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_div_fixup_f32", vdst, src0, src1, src2, modifier); }
    void v_div_fixup_f64(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_div_fixup_f64", vdst, src0, src1, src2, modifier); }
    void v_div_fmas_f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_div_fmas_f32", vdst, src0, src1, src2, modifier); }
    void v_div_fmas_f64(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_div_fmas_f64", vdst, src0, src1, src2, modifier); }
    void v_div_scale_f32(const operand_t & vdst, const operand_t & vcc, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_div_scale_f32", vdst, vcc, src0, src1, src2); }
    void v_div_scale_f64(const operand_t & vdst, const operand_t & vcc, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_div_scale_f64", vdst, vcc, src0, src1, src2); }
    void v_fma_f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_fma_f16", vdst, src0, src1, src2, modifier); }
    void v_fma_f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_fma_f32", vdst, src0, src1, src2, modifier); }
    void v_fma_f64(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_fma_f64", vdst, src0, src1, src2, modifier); }
    void v_fma_legacy_f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_fma_legacy_f32", vdst, src0, src1, src2, modifier); }
    void v_interp_p1ll_f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_interp_p1ll_f16", vdst, src0, src1, modifier); }
    void v_interp_p1lv_f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_interp_p1lv_f16", vdst, src0, src1, src2, modifier); }
    void v_interp_p2_f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_interp_p2_f16", vdst, src0, src1, src2, modifier); }
    void v_ldexp_f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_ldexp_f32", vdst, src0, src1, modifier); }
    void v_ldexp_f64(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_ldexp_f64", vdst, src0, src1, modifier); }
    void v_lerp_u8(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_lerp_u8", vdst, src0, src1, src2); }
    void v_lshl_add_u32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_lshl_add_u32", vdst, src0, src1, src2); }
    void v_lshl_or_b32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_lshl_or_b32", vdst, src0, src1, src2); }
    void v_lshlrev_b16(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_lshlrev_b16", vdst, src0, src1); }
    void v_lshlrev_b64(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_lshlrev_b64", vdst, src0, src1); }
    void v_lshrrev_b16(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_lshrrev_b16", vdst, src0, src1); }
    void v_lshrrev_b64(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_lshrrev_b64", vdst, src0, src1); }
    void v_mad_i16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_mad_i16", vdst, src0, src1, src2, modifier); }
    void v_mad_i32_i16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_mad_i32_i16", vdst, src0, src1, src2, modifier); }
    void v_mad_i32_i24(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_mad_i32_i24", vdst, src0, src1, src2, modifier); }
    void v_mad_i64_i32(const operand_t & vdst, const operand_t & sdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_mad_i64_i32", vdst, sdst, src0, src1, src2, modifier); }
    void v_mad_u16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_mad_u16", vdst, src0, src1, src2, modifier); }
    void v_mad_u32_u16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_mad_u32_u16", vdst, src0, src1, src2, modifier); }
    void v_mad_u32_u24(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_mad_u32_u24", vdst, src0, src1, src2, modifier); }
    void v_mad_u64_u32(const operand_t & vdst, const operand_t & sdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_mad_u64_u32", vdst, sdst, src0, src1, src2, modifier); }
    void v_max3_f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_max3_f16", vdst, src0, src1, src2, modifier); }
    void v_max3_f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_max3_f32", vdst, src0, src1, src2, modifier); }
    void v_max3_i16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_max3_i16", vdst, src0, src1, src2, modifier); }
    void v_max3_i32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_max3_i32", vdst, src0, src1, src2); }
    void v_max3_u16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_max3_u16", vdst, src0, src1, src2, modifier); }
    void v_max3_u32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_max3_u32", vdst, src0, src1, src2); }
    void v_max_f64(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_max_f64", vdst, src0, src1, modifier); }
    void v_max_i16(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_max_i16", vdst, src0, src1); }
    void v_max_u16(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_max_u16", vdst, src0, src1); }
    void v_mbcnt_hi_u32_b32(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_mbcnt_hi_u32_b32", vdst, src0, src1); }
    void v_mbcnt_lo_u32_b32(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_mbcnt_lo_u32_b32", vdst, src0, src1); }
    void v_med3_f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_med3_f16", vdst, src0, src1, src2, modifier); }
    void v_med3_f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_med3_f32", vdst, src0, src1, src2, modifier); }
    void v_med3_i16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_med3_i16", vdst, src0, src1, src2, modifier); }
    void v_med3_i32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_med3_i32", vdst, src0, src1, src2); }
    void v_med3_u16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_med3_u16", vdst, src0, src1, src2, modifier); }
    void v_med3_u32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_med3_u32", vdst, src0, src1, src2); }
    void v_min3_f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_min3_f16", vdst, src0, src1, src2, modifier); }
    void v_min3_f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_min3_f32", vdst, src0, src1, src2, modifier); }
    void v_min3_i16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_min3_i16", vdst, src0, src1, src2, modifier); }
    void v_min3_i32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_min3_i32", vdst, src0, src1, src2); }
    void v_min3_u16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_min3_u16", vdst, src0, src1, src2, modifier); }
    void v_min3_u32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_min3_u32", vdst, src0, src1, src2); }
    void v_min_f64(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_min_f64", vdst, src0, src1, modifier); }
    void v_min_i16(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_min_i16", vdst, src0, src1); }
    void v_min_u16(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_min_u16", vdst, src0, src1); }
    void v_mqsad_pk_u16_u8(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_mqsad_pk_u16_u8", vdst, src0, src1, src2, modifier); }
    void v_mqsad_u32_u8(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_mqsad_u32_u8", vdst, src0, src1, src2, modifier); }
    void v_msad_u8(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_msad_u8", vdst, src0, src1, src2, modifier); }
    void v_mul_f64(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_mul_f64", vdst, src0, src1, modifier); }
    void v_mul_hi_i32(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_mul_hi_i32", vdst, src0, src1); }
    void v_mul_hi_u32(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_mul_hi_u32", vdst, src0, src1); }
    void v_mul_lo_u16(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_mul_lo_u16", vdst, src0, src1); }
    void v_mul_lo_u32(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_mul_lo_u32", vdst, src0, src1); }
    void v_mullit_f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_mullit_f32", vdst, src0, src1, src2, modifier); }
    void v_or3_b32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_or3_b32", vdst, src0, src1, src2); }
    void v_pack_b32_f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_pack_b32_f16", vdst, src0, src1, modifier); }
    void v_perm_b32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_perm_b32", vdst, src0, src1, src2); }
    void v_permlane16_b32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_permlane16_b32", vdst, src0, src1, src2, modifier); }
    void v_permlanex16_b32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_permlanex16_b32", vdst, src0, src1, src2, modifier); }
    void v_qsad_pk_u16_u8(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_qsad_pk_u16_u8", vdst, src0, src1, src2, modifier); }
    void v_readlane_b32(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_readlane_b32", vdst, src0, src1); }
    void v_sad_hi_u8(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_sad_hi_u8", vdst, src0, src1, src2, modifier); }
    void v_sad_u16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_sad_u16", vdst, src0, src1, src2, modifier); }
    void v_sad_u32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_sad_u32", vdst, src0, src1, src2, modifier); }
    void v_sad_u8(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_sad_u8", vdst, src0, src1, src2, modifier); }
    void v_sub_co_u32(const operand_t & vdst, const operand_t & sdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_sub_co_u32", vdst, sdst, src0, src1, operand_t(), modifier); }
    void v_sub_nc_i16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_sub_nc_i16", vdst, src0, src1, modifier); }
    void v_sub_nc_i32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_sub_nc_i32", vdst, src0, src1, modifier); }
    void v_sub_nc_u16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_sub_nc_u16", vdst, src0, src1, modifier); }
    void v_subrev_co_u32(const operand_t & vdst, const operand_t & sdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_subrev_co_u32", vdst, sdst, src0, src1, operand_t(), modifier); }
    void v_trig_preop_f64(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3("v_trig_preop_f64", vdst, src0, src1, modifier); }
    void v_writelane_b32(const operand_t & vdst, const operand_t & src0, const operand_t & src1) { r_vop3("v_writelane_b32", vdst, src0, src1); }
    void v_xad_u32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_xad_u32", vdst, src0, src1, src2); }
    void v_xor3_b32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2) { r_vop3("v_xor3_b32", vdst, src0, src1, src2); }


    // VOP3P
    void v_dot2_f32_f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_dot2_f32_f16", vdst, src0, src1, src2, modifier); }
    void v_dot2_i32_i16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_dot2_i32_i16", vdst, src0, src1, src2, modifier); }
    void v_dot2_u32_u16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_dot2_u32_u16", vdst, src0, src1, src2, modifier); }
    void v_dot4_i32_i8(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_dot4_i32_i8", vdst, src0, src1, src2, modifier); }
    void v_dot4_u32_u8(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_dot4_u32_u8", vdst, src0, src1, src2, modifier); }
    void v_dot8_i32_i4(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_dot8_i32_i4", vdst, src0, src1, src2, modifier); }
    void v_dot8_u32_u4(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_dot8_u32_u4", vdst, src0, src1, src2, modifier); }
    void v_fma_mix_f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_fma_mix_f32", vdst, src0, src1, src2, modifier); }
    void v_fma_mixhi_f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_fma_mixhi_f16", vdst, src0, src1, src2, modifier); }
    void v_fma_mixlo_f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_fma_mixlo_f16", vdst, src0, src1, src2, modifier); }
    void v_pk_add_f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_pk_add_f16", vdst, src0, src1, operand_t(), modifier); }
    void v_pk_add_i16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_pk_add_i16", vdst, src0, src1, operand_t(), modifier); }
    void v_pk_add_u16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_pk_add_u16", vdst, src0, src1, operand_t(), modifier); }
    void v_pk_fma_f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_pk_fma_f16", vdst, src0, src1, src2, modifier); }
    void v_pk_lshlrev_b16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_pk_lshlrev_b16", vdst, src0, src1, operand_t(), modifier); }
    void v_pk_lshrrev_b16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_pk_lshrrev_b16", vdst, src0, src1, operand_t(), modifier); }
    void v_pk_mad_i16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_pk_mad_i16", vdst, src0, src1, src2, modifier); }
    void v_pk_mad_u16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_pk_mad_u16", vdst, src0, src1, src2, modifier); }
    void v_pk_max_f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_pk_max_f16", vdst, src0, src1, operand_t(), modifier); }
    void v_pk_max_i16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_pk_max_i16", vdst, src0, src1, operand_t(), modifier); }
    void v_pk_max_u16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_pk_max_u16", vdst, src0, src1, operand_t(), modifier); }
    void v_pk_min_f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_pk_min_f16", vdst, src0, src1, operand_t(), modifier); }
    void v_pk_min_i16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_pk_min_i16", vdst, src0, src1, operand_t(), modifier); }
    void v_pk_min_u16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_pk_min_u16", vdst, src0, src1, operand_t(), modifier); }
    void v_pk_mul_f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_pk_mul_f16", vdst, src0, src1, operand_t(), modifier); }
    void v_pk_mul_lo_u16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_pk_mul_lo_u16", vdst, src0, src1, operand_t(), modifier); }
    void v_pk_sub_i16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_pk_sub_i16", vdst, src0, src1, operand_t(), modifier); }
    void v_pk_sub_u16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_pk_sub_u16", vdst, src0, src1, operand_t(), modifier); }
    void v_mfma_f32_16x16x16f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_mfma_f32_16x16x16f16", vdst, src0, src1, src2, modifier); }
    void v_mfma_f32_16x16x1f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_mfma_f32_16x16x1f32", vdst, src0, src1, src2, modifier); }
    void v_mfma_f32_16x16x2bf16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_mfma_f32_16x16x2bf16", vdst, src0, src1, src2, modifier); }
    void v_mfma_f32_16x16x4f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_mfma_f32_16x16x4f16", vdst, src0, src1, src2, modifier); }
    void v_mfma_f32_16x16x4f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_mfma_f32_16x16x4f32", vdst, src0, src1, src2, modifier); }
    void v_mfma_f32_16x16x8bf16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_mfma_f32_16x16x8bf16", vdst, src0, src1, src2, modifier); }
    void v_mfma_f32_32x32x1f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_mfma_f32_32x32x1f32", vdst, src0, src1, src2, modifier); }
    void v_mfma_f32_32x32x2bf16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_mfma_f32_32x32x2bf16", vdst, src0, src1, src2, modifier); }
    void v_mfma_f32_32x32x2f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_mfma_f32_32x32x2f32", vdst, src0, src1, src2, modifier); }
    void v_mfma_f32_32x32x4bf16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_mfma_f32_32x32x4bf16", vdst, src0, src1, src2, modifier); }
    void v_mfma_f32_32x32x4f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_mfma_f32_32x32x4f16", vdst, src0, src1, src2, modifier); }
    void v_mfma_f32_32x32x8f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_mfma_f32_32x32x8f16", vdst, src0, src1, src2, modifier); }
    void v_mfma_f32_4x4x1f32(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_mfma_f32_4x4x1f32", vdst, src0, src1, src2, modifier); }
    void v_mfma_f32_4x4x2bf16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_mfma_f32_4x4x2bf16", vdst, src0, src1, src2, modifier); }
    void v_mfma_f32_4x4x4f16(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_mfma_f32_4x4x4f16", vdst, src0, src1, src2, modifier); }
    void v_mfma_i32_16x16x16i8(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_mfma_i32_16x16x16i8", vdst, src0, src1, src2, modifier); }
    void v_mfma_i32_16x16x4i8(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_mfma_i32_16x16x4i8", vdst, src0, src1, src2, modifier); }
    void v_mfma_i32_32x32x4i8(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_mfma_i32_32x32x4i8", vdst, src0, src1, src2, modifier); }
    void v_mfma_i32_32x32x8i8(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_mfma_i32_32x32x8i8", vdst, src0, src1, src2, modifier); }
    void v_mfma_i32_4x4x4i8(const operand_t & vdst, const operand_t & src0, const operand_t & src1, const operand_t & src2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3p("v_mfma_i32_4x4x4i8", vdst, src0, src1, src2, modifier); }

#define IGNORE_VCC(name, dst, src0, vsrc1, modifier)                                                    \
    if(emitter.get_emitter_type() == EMITTER_LLVM_HSA && dst.get_type() == OPERAND_REGISTER_HWREG)      \
        r_vop3c(name, operand_t(), src0, vsrc1, modifier);                                              \
    else                                                                                                \
        r_vop3c(name, dst, src0, vsrc1, modifier);                                                      

    // VOP3C
    void v_cmp_class_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_class_f16", dst, src0, vsrc1, modifier); }
    void v_cmp_class_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_class_f32", dst, src0, vsrc1, modifier); }
    void v_cmp_class_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_class_f64", dst, src0, vsrc1, modifier); }
    void v_cmp_eq_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_eq_f16", dst, src0, vsrc1, modifier); }
    void v_cmp_eq_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_eq_f32", dst, src0, vsrc1, modifier); }
    void v_cmp_eq_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_eq_f64", dst, src0, vsrc1, modifier); }
    void v_cmp_eq_i16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_eq_i16", dst, src0, vsrc1, modifier); }
    void v_cmp_eq_i32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_eq_i32", dst, src0, vsrc1, modifier); }
    void v_cmp_eq_i64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_eq_i64", dst, src0, vsrc1, modifier); }
    void v_cmp_eq_u16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_eq_u16", dst, src0, vsrc1, modifier); }
    void v_cmp_eq_u32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_eq_u32", dst, src0, vsrc1, modifier); }
    void v_cmp_eq_u64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_eq_u64", dst, src0, vsrc1, modifier); }
    void v_cmp_f_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_f_f16", dst, src0, vsrc1, modifier); }
    void v_cmp_f_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_f_f32", dst, src0, vsrc1, modifier); }
    void v_cmp_f_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_f_f64", dst, src0, vsrc1, modifier); }
    void v_cmp_f_i32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_f_i32", dst, src0, vsrc1, modifier); }
    void v_cmp_f_i64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_f_i64", dst, src0, vsrc1, modifier); }
    void v_cmp_f_u32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_f_u32", dst, src0, vsrc1, modifier); }
    void v_cmp_f_u64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_f_u64", dst, src0, vsrc1, modifier); }
    void v_cmp_ge_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_ge_f16", dst, src0, vsrc1, modifier); }
    void v_cmp_ge_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_ge_f32", dst, src0, vsrc1, modifier); }
    void v_cmp_ge_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_ge_f64", dst, src0, vsrc1, modifier); }
    void v_cmp_ge_i16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_ge_i16", dst, src0, vsrc1, modifier); }
    void v_cmp_ge_i32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_ge_i32", dst, src0, vsrc1, modifier); }
    void v_cmp_ge_i64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_ge_i64", dst, src0, vsrc1, modifier); }
    void v_cmp_ge_u16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_ge_u16", dst, src0, vsrc1, modifier); }
    void v_cmp_ge_u32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_ge_u32", dst, src0, vsrc1, modifier); }
    void v_cmp_ge_u64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_ge_u64", dst, src0, vsrc1, modifier); }
    void v_cmp_gt_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_gt_f16", dst, src0, vsrc1, modifier); }
    void v_cmp_gt_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_gt_f32", dst, src0, vsrc1, modifier); }
    void v_cmp_gt_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_gt_f64", dst, src0, vsrc1, modifier); }
    void v_cmp_gt_i16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_gt_i16", dst, src0, vsrc1, modifier); }
    void v_cmp_gt_i32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_gt_i32", dst, src0, vsrc1, modifier); }
    void v_cmp_gt_i64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_gt_i64", dst, src0, vsrc1, modifier); }
    void v_cmp_gt_u16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_gt_u16", dst, src0, vsrc1, modifier); }
    void v_cmp_gt_u32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_gt_u32", dst, src0, vsrc1, modifier); }
    void v_cmp_gt_u64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_gt_u64", dst, src0, vsrc1, modifier); }
    void v_cmp_le_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_le_f16", dst, src0, vsrc1, modifier); }
    void v_cmp_le_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_le_f32", dst, src0, vsrc1, modifier); }
    void v_cmp_le_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_le_f64", dst, src0, vsrc1, modifier); }
    void v_cmp_le_i16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_le_i16", dst, src0, vsrc1, modifier); }
    void v_cmp_le_i32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_le_i32", dst, src0, vsrc1, modifier); }
    void v_cmp_le_i64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_le_i64", dst, src0, vsrc1, modifier); }
    void v_cmp_le_u16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_le_u16", dst, src0, vsrc1, modifier); }
    void v_cmp_le_u32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_le_u32", dst, src0, vsrc1, modifier); }
    void v_cmp_le_u64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_le_u64", dst, src0, vsrc1, modifier); }
    void v_cmp_lg_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_lg_f16", dst, src0, vsrc1, modifier); }
    void v_cmp_lg_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_lg_f32", dst, src0, vsrc1, modifier); }
    void v_cmp_lg_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_lg_f64", dst, src0, vsrc1, modifier); }
    void v_cmp_lt_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_lt_f16", dst, src0, vsrc1, modifier); }
    void v_cmp_lt_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_lt_f32", dst, src0, vsrc1, modifier); }
    void v_cmp_lt_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_lt_f64", dst, src0, vsrc1, modifier); }
    void v_cmp_lt_i16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_lt_i16", dst, src0, vsrc1, modifier); }
    void v_cmp_lt_i32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_lt_i32", dst, src0, vsrc1, modifier); }
    void v_cmp_lt_i64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_lt_i64", dst, src0, vsrc1, modifier); }
    void v_cmp_lt_u16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_lt_u16", dst, src0, vsrc1, modifier); }
    void v_cmp_lt_u32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_lt_u32", dst, src0, vsrc1, modifier); }
    void v_cmp_lt_u64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_lt_u64", dst, src0, vsrc1, modifier); }
    void v_cmp_ne_i16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_ne_i16", dst, src0, vsrc1, modifier); }
    void v_cmp_ne_i32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_ne_i32", dst, src0, vsrc1, modifier); }
    void v_cmp_ne_i64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_ne_i64", dst, src0, vsrc1, modifier); }
    void v_cmp_ne_u16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_ne_u16", dst, src0, vsrc1, modifier); }
    void v_cmp_ne_u32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_ne_u32", dst, src0, vsrc1, modifier); }
    void v_cmp_ne_u64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_ne_u64", dst, src0, vsrc1, modifier); }
    void v_cmp_neq_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_neq_f16", dst, src0, vsrc1, modifier); }
    void v_cmp_neq_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_neq_f32", dst, src0, vsrc1, modifier); }
    void v_cmp_neq_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_neq_f64", dst, src0, vsrc1, modifier); }
    void v_cmp_nge_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_nge_f16", dst, src0, vsrc1, modifier); }
    void v_cmp_nge_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_nge_f32", dst, src0, vsrc1, modifier); }
    void v_cmp_nge_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_nge_f64", dst, src0, vsrc1, modifier); }
    void v_cmp_ngt_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_ngt_f16", dst, src0, vsrc1, modifier); }
    void v_cmp_ngt_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_ngt_f32", dst, src0, vsrc1, modifier); }
    void v_cmp_ngt_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_ngt_f64", dst, src0, vsrc1, modifier); }
    void v_cmp_nle_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_nle_f16", dst, src0, vsrc1, modifier); }
    void v_cmp_nle_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_nle_f32", dst, src0, vsrc1, modifier); }
    void v_cmp_nle_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_nle_f64", dst, src0, vsrc1, modifier); }
    void v_cmp_nlg_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_nlg_f16", dst, src0, vsrc1, modifier); }
    void v_cmp_nlg_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_nlg_f32", dst, src0, vsrc1, modifier); }
    void v_cmp_nlg_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_nlg_f64", dst, src0, vsrc1, modifier); }
    void v_cmp_nlt_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_nlt_f16", dst, src0, vsrc1, modifier); }
    void v_cmp_nlt_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_nlt_f32", dst, src0, vsrc1, modifier); }
    void v_cmp_nlt_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_nlt_f64", dst, src0, vsrc1, modifier); }
    void v_cmp_o_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_o_f16", dst, src0, vsrc1, modifier); }
    void v_cmp_o_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_o_f32", dst, src0, vsrc1, modifier); }
    void v_cmp_o_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_o_f64", dst, src0, vsrc1, modifier); }
    void v_cmp_t_i32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_t_i32", dst, src0, vsrc1, modifier); }
    void v_cmp_t_i64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_t_i64", dst, src0, vsrc1, modifier); }
    void v_cmp_t_u32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_t_u32", dst, src0, vsrc1, modifier); }
    void v_cmp_t_u64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_t_u64", dst, src0, vsrc1, modifier); }
    void v_cmp_tru_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_tru_f16", dst, src0, vsrc1, modifier); }
    void v_cmp_tru_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_tru_f32", dst, src0, vsrc1, modifier); }
    void v_cmp_tru_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_tru_f64", dst, src0, vsrc1, modifier); }
    void v_cmp_u_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_u_f16", dst, src0, vsrc1, modifier); }
    void v_cmp_u_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_u_f32", dst, src0, vsrc1, modifier); }
    void v_cmp_u_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmp_u_f64", dst, src0, vsrc1, modifier); }
    void v_cmpx_class_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_class_f16", dst, src0, vsrc1, modifier); }
    void v_cmpx_class_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_class_f32", dst, src0, vsrc1, modifier); }
    void v_cmpx_class_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_class_f64", dst, src0, vsrc1, modifier); }
    void v_cmpx_eq_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_eq_f16", dst, src0, vsrc1, modifier); }
    void v_cmpx_eq_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_eq_f32", dst, src0, vsrc1, modifier); }
    void v_cmpx_eq_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_eq_f64", dst, src0, vsrc1, modifier); }
    void v_cmpx_eq_i16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_eq_i16", dst, src0, vsrc1, modifier); }
    void v_cmpx_eq_i32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_eq_i32", dst, src0, vsrc1, modifier); }
    void v_cmpx_eq_i64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_eq_i64", dst, src0, vsrc1, modifier); }
    void v_cmpx_eq_u16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_eq_u16", dst, src0, vsrc1, modifier); }
    void v_cmpx_eq_u32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_eq_u32", dst, src0, vsrc1, modifier); }
    void v_cmpx_eq_u64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_eq_u64", dst, src0, vsrc1, modifier); }
    void v_cmpx_f_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_f_f16", dst, src0, vsrc1, modifier); }
    void v_cmpx_f_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_f_f32", dst, src0, vsrc1, modifier); }
    void v_cmpx_f_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_f_f64", dst, src0, vsrc1, modifier); }
    void v_cmpx_f_i32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_f_i32", dst, src0, vsrc1, modifier); }
    void v_cmpx_f_i64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_f_i64", dst, src0, vsrc1, modifier); }
    void v_cmpx_f_u32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_f_u32", dst, src0, vsrc1, modifier); }
    void v_cmpx_f_u64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_f_u64", dst, src0, vsrc1, modifier); }
    void v_cmpx_ge_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_ge_f16", dst, src0, vsrc1, modifier); }
    void v_cmpx_ge_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_ge_f32", dst, src0, vsrc1, modifier); }
    void v_cmpx_ge_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_ge_f64", dst, src0, vsrc1, modifier); }
    void v_cmpx_ge_i16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_ge_i16", dst, src0, vsrc1, modifier); }
    void v_cmpx_ge_i32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_ge_i32", dst, src0, vsrc1, modifier); }
    void v_cmpx_ge_i64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_ge_i64", dst, src0, vsrc1, modifier); }
    void v_cmpx_ge_u16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_ge_u16", dst, src0, vsrc1, modifier); }
    void v_cmpx_ge_u32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_ge_u32", dst, src0, vsrc1, modifier); }
    void v_cmpx_ge_u64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_ge_u64", dst, src0, vsrc1, modifier); }
    void v_cmpx_gt_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_gt_f16", dst, src0, vsrc1, modifier); }
    void v_cmpx_gt_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_gt_f32", dst, src0, vsrc1, modifier); }
    void v_cmpx_gt_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_gt_f64", dst, src0, vsrc1, modifier); }
    void v_cmpx_gt_i16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_gt_i16", dst, src0, vsrc1, modifier); }
    void v_cmpx_gt_i32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_gt_i32", dst, src0, vsrc1, modifier); }
    void v_cmpx_gt_i64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_gt_i64", dst, src0, vsrc1, modifier); }
    void v_cmpx_gt_u16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_gt_u16", dst, src0, vsrc1, modifier); }
    void v_cmpx_gt_u32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_gt_u32", dst, src0, vsrc1, modifier); }
    void v_cmpx_gt_u64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_gt_u64", dst, src0, vsrc1, modifier); }
    void v_cmpx_le_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_le_f16", dst, src0, vsrc1, modifier); }
    void v_cmpx_le_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_le_f32", dst, src0, vsrc1, modifier); }
    void v_cmpx_le_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_le_f64", dst, src0, vsrc1, modifier); }
    void v_cmpx_le_i16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_le_i16", dst, src0, vsrc1, modifier); }
    void v_cmpx_le_i32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_le_i32", dst, src0, vsrc1, modifier); }
    void v_cmpx_le_i64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_le_i64", dst, src0, vsrc1, modifier); }
    void v_cmpx_le_u16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_le_u16", dst, src0, vsrc1, modifier); }
    void v_cmpx_le_u32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_le_u32", dst, src0, vsrc1, modifier); }
    void v_cmpx_le_u64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_le_u64", dst, src0, vsrc1, modifier); }
    void v_cmpx_lg_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_lg_f16", dst, src0, vsrc1, modifier); }
    void v_cmpx_lg_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_lg_f32", dst, src0, vsrc1, modifier); }
    void v_cmpx_lg_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_lg_f64", dst, src0, vsrc1, modifier); }
    void v_cmpx_lt_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_lt_f16", dst, src0, vsrc1, modifier); }
    void v_cmpx_lt_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_lt_f32", dst, src0, vsrc1, modifier); }
    void v_cmpx_lt_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_lt_f64", dst, src0, vsrc1, modifier); }
    void v_cmpx_lt_i16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_lt_i16", dst, src0, vsrc1, modifier); }
    void v_cmpx_lt_i32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_lt_i32", dst, src0, vsrc1, modifier); }
    void v_cmpx_lt_i64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_lt_i64", dst, src0, vsrc1, modifier); }
    void v_cmpx_lt_u16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_lt_u16", dst, src0, vsrc1, modifier); }
    void v_cmpx_lt_u32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_lt_u32", dst, src0, vsrc1, modifier); }
    void v_cmpx_lt_u64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_lt_u64", dst, src0, vsrc1, modifier); }
    void v_cmpx_ne_i16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_ne_i16", dst, src0, vsrc1, modifier); }
    void v_cmpx_ne_i32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_ne_i32", dst, src0, vsrc1, modifier); }
    void v_cmpx_ne_i64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_ne_i64", dst, src0, vsrc1, modifier); }
    void v_cmpx_ne_u16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_ne_u16", dst, src0, vsrc1, modifier); }
    void v_cmpx_ne_u32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_ne_u32", dst, src0, vsrc1, modifier); }
    void v_cmpx_ne_u64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_ne_u64", dst, src0, vsrc1, modifier); }
    void v_cmpx_neq_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_neq_f16", dst, src0, vsrc1, modifier); }
    void v_cmpx_neq_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_neq_f32", dst, src0, vsrc1, modifier); }
    void v_cmpx_neq_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_neq_f64", dst, src0, vsrc1, modifier); }
    void v_cmpx_nge_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_nge_f16", dst, src0, vsrc1, modifier); }
    void v_cmpx_nge_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_nge_f32", dst, src0, vsrc1, modifier); }
    void v_cmpx_nge_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_nge_f64", dst, src0, vsrc1, modifier); }
    void v_cmpx_ngt_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_ngt_f16", dst, src0, vsrc1, modifier); }
    void v_cmpx_ngt_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_ngt_f32", dst, src0, vsrc1, modifier); }
    void v_cmpx_ngt_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_ngt_f64", dst, src0, vsrc1, modifier); }
    void v_cmpx_nle_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_nle_f16", dst, src0, vsrc1, modifier); }
    void v_cmpx_nle_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_nle_f32", dst, src0, vsrc1, modifier); }
    void v_cmpx_nle_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_nle_f64", dst, src0, vsrc1, modifier); }
    void v_cmpx_nlg_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_nlg_f16", dst, src0, vsrc1, modifier); }
    void v_cmpx_nlg_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_nlg_f32", dst, src0, vsrc1, modifier); }
    void v_cmpx_nlg_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_nlg_f64", dst, src0, vsrc1, modifier); }
    void v_cmpx_nlt_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_nlt_f16", dst, src0, vsrc1, modifier); }
    void v_cmpx_nlt_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_nlt_f32", dst, src0, vsrc1, modifier); }
    void v_cmpx_nlt_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_nlt_f64", dst, src0, vsrc1, modifier); }
    void v_cmpx_o_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_o_f16", dst, src0, vsrc1, modifier); }
    void v_cmpx_o_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_o_f32", dst, src0, vsrc1, modifier); }
    void v_cmpx_o_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_o_f64", dst, src0, vsrc1, modifier); }
    void v_cmpx_t_i32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_t_i32", dst, src0, vsrc1, modifier); }
    void v_cmpx_t_i64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_t_i64", dst, src0, vsrc1, modifier); }
    void v_cmpx_t_u32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_t_u32", dst, src0, vsrc1, modifier); }
    void v_cmpx_t_u64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_t_u64", dst, src0, vsrc1, modifier); }
    void v_cmpx_tru_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_tru_f16", dst, src0, vsrc1, modifier); }
    void v_cmpx_tru_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_tru_f32", dst, src0, vsrc1, modifier); }
    void v_cmpx_tru_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_tru_f64", dst, src0, vsrc1, modifier); }
    void v_cmpx_u_f16(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_u_f16", dst, src0, vsrc1, modifier); }
    void v_cmpx_u_f32(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_u_f32", dst, src0, vsrc1, modifier); }
    void v_cmpx_u_f64(const operand_t & dst, const operand_t & src0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { IGNORE_VCC("v_cmpx_u_f64", dst, src0, vsrc1, modifier); }

    const emitter_type get_emitter_type() const { return emitter.get_emitter_type(); }
    const target_info_t & get_target_info() const { return ti; }
    const uint32_t get_indent() const { return indent; }
    const std::vector<instruction_t> & get_instruction_sequence() const { return instruction_sequence; }
    
#if GXCO_DIRECT_EMIT
    //
#else
    void record(const instruction_t & inst) { instruction_sequence.emplace_back(std::move(inst)); }   // TODO: emplace_back with forwarding
#endif

protected:
    const target_info_t & ti;
    emitter_t & emitter;
    uint32_t indent;

    std::vector<instruction_t> instruction_sequence;
    
    void r_ds(const std::string & name, const operand_t & vdst, const operand_t & vsrc0, const operand_t & vsrc1, const operand_t & vsrc2, const std::initializer_list<modifier_t> & modifier = {}) { record(instruction_t(ENC_DS, name, {vdst}, {vsrc0, vsrc1, vsrc2}, modifier, indent)); }
    void r_flat(const std::string & name, const operand_t & vdst, const operand_t & vsrc0, const operand_t & vsrc1, const operand_t & vsrc2, const std::initializer_list<modifier_t> & modifier = {}) { record(instruction_t(ENC_FLAT, name, {vdst}, {vsrc0, vsrc1, vsrc2}, modifier, indent)); }
    void r_mubuf(const std::string & name, const operand_t & vdst, const operand_t & vsrc0, const operand_t & vsrc1, const operand_t & vsrc2, const operand_t & vsrc3, const std::initializer_list<modifier_t> & modifier = {}) { record(instruction_t(ENC_MUBUF, name, {vdst}, {vsrc0, vsrc1, vsrc2, vsrc3}, modifier, indent)); }

    void r_smem(const std::string & name, const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1, const operand_t & ssrc2, const std::initializer_list<modifier_t> & modifier = {})  { record(instruction_t(ENC_SMEM, name, {sdst}, {ssrc0, ssrc1, ssrc2}, modifier, indent)); }

    void r_sop1(const std::string & name, const operand_t & sdst, const operand_t & ssrc) { record(instruction_t(ENC_SOP1, name, {sdst}, {ssrc}, {}, indent)); }
    void r_sop2(const std::string & name, const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) { record(instruction_t(ENC_SOP2, name, {sdst}, {ssrc0, ssrc1}, {}, indent)); }
    void r_sopc(const std::string & name, const operand_t & ssrc0, const operand_t & ssrc1) { record(instruction_t(ENC_SOPC, name, {}, {ssrc0, ssrc1}, {}, indent)); }
    void r_sopk(const std::string & name, const operand_t & sdst, const operand_t & ssrc0, const operand_t & ssrc1) { record(instruction_t(ENC_SOPK, name, {sdst}, {ssrc0, ssrc1}, {}, indent)); }
    void r_sopp(const std::string & name, const operand_t & ssrc) { record(instruction_t(ENC_SOPP, name, {}, {ssrc}, {}, indent)); }

    void r_vop1(const std::string & name, const operand_t & vdst, const operand_t & vsrc, const std::initializer_list<modifier_t> & modifier = {}) {record(instruction_t(ENC_VOP1, name, {vdst}, {vsrc}, modifier, indent)); }
    void r_vop2(const std::string & name, const operand_t & vdst0, const operand_t & vdst1, const operand_t & vsrc0, const operand_t & vsrc1, const operand_t & vsrc2, const std::initializer_list<modifier_t> & modifier = {}) { record(instruction_t(ENC_VOP2, name, {vdst0, vdst1}, {vsrc0, vsrc1, vsrc2}, modifier, indent)); }
    void r_vop2(const std::string & name, const operand_t & vdst, const operand_t & vsrc0, const operand_t & vsrc1, const operand_t & vsrc2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2(name, vdst, operand_t(), vsrc0, vsrc1, vsrc2, modifier);}
    void r_vop2(const std::string & name, const operand_t & vdst, const operand_t & vsrc0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop2(name, vdst, operand_t(), vsrc0, vsrc1, operand_t(), modifier);}
    void r_vop3(const std::string & name, const operand_t & vdst0, const operand_t & vdst1, const operand_t & vsrc0, const operand_t & vsrc1, const operand_t & vsrc2, const std::initializer_list<modifier_t> & modifier = {}) { record(instruction_t(ENC_VOP3, name, {vdst0, vdst1}, {vsrc0, vsrc1, vsrc2}, modifier, indent)); }
    void r_vop3(const std::string & name, const operand_t & vdst, const operand_t & vsrc0, const operand_t & vsrc1, const operand_t & vsrc2, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3(name, vdst, operand_t(), vsrc0, vsrc1, vsrc2, modifier); }
    void r_vop3(const std::string & name, const operand_t & vdst, const operand_t & vsrc0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3(name, vdst, operand_t(), vsrc0, vsrc1, operand_t(), modifier); }
    void r_vop3p(const std::string & name, const operand_t & vdst, const operand_t & vsrc0, const operand_t & vsrc1, const operand_t & vsrc2, const std::initializer_list<modifier_t> & modifier = {}) { record(instruction_t(ENC_VOP3P, name, {vdst}, {vsrc0, vsrc1, vsrc2}, modifier, indent)); }
    void r_vop3c(const std::string & name, const operand_t & dst, const operand_t & vsrc0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { record(instruction_t(ENC_VOP3C, name, {dst}, {vsrc0, vsrc1}, modifier, indent)); }
    void r_vop3c(const std::string & name, const operand_t & vsrc0, const operand_t & vsrc1, const std::initializer_list<modifier_t> & modifier = {}) { r_vop3c(name, operand_t(), vsrc0, vsrc1, modifier); }

};