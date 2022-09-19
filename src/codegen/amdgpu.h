#include <string>
#include <cassert>
#include <unordered_map>
#include "gxco/gxco.h"
typedef enum{
    AMDGPU_PRECISION_UNKNOWN= (1 << 19),
    AMDGPU_PRECISION_FP32   = (0 << 20),
    AMDGPU_PRECISION_FP16   = (1 << 20),
    AMDGPU_PRECISION_BF16   = (2 << 20),
    AMDGPU_PRECISION_INT8   = (3 << 20),
    AMDGPU_PRECISION_UINT8  = (4 << 20),
    AMDGPU_PRECISION_INT4   = (5 << 20),
    AMDGPU_PRECISION_UINT4  = (6 << 20),
    AMDGPU_PRECISION_INT16  = (7 << 20),
    AMDGPU_PRECISION_UINT16 = (8 << 20),
} amdgpu_precision_t;

//
typedef enum{
    AMDGPU_CODEOBJECT_UNKNOWN = (0 << 28),
    AMDGPU_CODEOBJECT_V2      = (2 << 28),
    AMDGPU_CODEOBJECT_V3      = (3 << 28),
} amdgpu_codeobj_t;

typedef enum{
    AMDGPU_ARCH_UNKNOWN     = 0,
    AMDGPU_ARCH_GFX900      = 900,
    AMDGPU_ARCH_GFX906      = 906,
    AMDGPU_ARCH_GFX908      = 908,
    AMDGPU_ARCH_GFX90A      = 910,
    AMDGPU_ARCH_GFX1030     = 1030,
    AMDGPU_ARCH_GFX1100     = 1100,
} amdgpu_arch_t;

#define AMDGPU_WAVE_SIZE         64
#define AMDGPU_XDLOPS_LANEGROUP_GRANULARITY_M  4
#define AMDGPU_XDLOPS_LANEGROUP_GRANULARITY_N  64

class _dict_with_default_t{
public:
    _dict_with_default_t(const std::unordered_map<std::string, int> & dict_) : dict(dict_) {}
    int operator()(const std::string &key, const int &default_value){
        if (dict.empty())
        {
            return default_value;
        }
        /* COMPATI contains is c++ 20 feature
         * if (dict.contains(key))
         */

        if (dict.find(key) != dict.end())
        {
            return dict.at(key);
        }
        return default_value;        
    }
private:
    std::unordered_map<std::string, int> dict;
};

gxco::arch_t amdgpu_string_to_arch(const std::string &amdgpu_arch_string){
    // if (amdgpu_arch_string == "gfx900")
    //     return AMDGPU_ARCH_GFX900;
    if (amdgpu_arch_string == "gfx906")
        return gxco::GFX906;
    if (amdgpu_arch_string == "gfx908")
        return gxco::GFX908;
    if (amdgpu_arch_string == "gfx90a")
        return gxco::GFX90A;
    if (amdgpu_arch_string == "gfx1030")
        return gxco::GFX1030;
    if (amdgpu_arch_string == "gfx1100")
        return gxco::GFX1100;
    return gxco::AMDGPU_ARCH_UNKNOWN;
}

std::string amdgpu_arch_to_string(const gxco::arch_t &amdgpu_arch){
    // if (amdgpu_arch_string == "gfx900")
    //     return AMDGPU_ARCH_GFX900;
    if (amdgpu_arch == gxco::GFX906)
        return "gfx906";
    if (amdgpu_arch == gxco::GFX908)
        return "gfx908";
    if (amdgpu_arch == gxco::GFX90A)
        return "gfx90a";
    if (amdgpu_arch == gxco::GFX1030)
        return "gfx1030";
    if (amdgpu_arch == gxco::GFX1100)
        return "gfx1100";

    return "AMDGPU_ARCH_UNKNOWN";
}

gxco::hsa_code_object_version amdgpu_string_to_codeobj(const std::string &amdgpu_codeobj_string){
    if(amdgpu_codeobj_string == "cov2")
        return gxco::HSA_CODE_OBJECT_V2;
    if(amdgpu_codeobj_string == "cov3")
        return gxco::HSA_CODE_OBJECT_V3;
    return gxco::AMDGPU_CODEOBJECT_UNKNOWN;
}

const std::string amdgpu_codeobj_to_string(const gxco::hsa_code_object_version &amdgpu_codeobj){
    if(amdgpu_codeobj == gxco::HSA_CODE_OBJECT_V2)
        return "cov2";
    if(amdgpu_codeobj == gxco::HSA_CODE_OBJECT_V3)
        return "cov3";
    if(amdgpu_codeobj == gxco::HSA_CODE_OBJECT_V4)
        return "cov4";
    return "cov_unknown";
}

const std::string amdgpu_precision_to_string(const amdgpu_precision_t &amdgpu_precision){
    if (amdgpu_precision == AMDGPU_PRECISION_FP32)
        return "fp32";
    if (amdgpu_precision == AMDGPU_PRECISION_FP16)
        return "fp16";
    if (amdgpu_precision == AMDGPU_PRECISION_BF16)
        return "bf16";
    if (amdgpu_precision == AMDGPU_PRECISION_INT8)
        return "int8";
    if (amdgpu_precision == AMDGPU_PRECISION_UINT8)
        return "uint8";
    if (amdgpu_precision == AMDGPU_PRECISION_INT4)
        return "int4";
    if (amdgpu_precision == AMDGPU_PRECISION_UINT4)
        return "uint4";
    if (amdgpu_precision == AMDGPU_PRECISION_INT16)
        return "int16";
    if (amdgpu_precision == AMDGPU_PRECISION_UINT16)
        return "uint16";
    return "amdgpu_precision_unknown";
}

amdgpu_precision_t amdgpu_string_to_precision(const std::string &amdgpu_precision_string){
    if (amdgpu_precision_string == "fp32")
        return AMDGPU_PRECISION_FP32;
    if (amdgpu_precision_string == "fp16")
        return AMDGPU_PRECISION_FP16;
    if (amdgpu_precision_string == "bf16")
        return AMDGPU_PRECISION_BF16;
    if (amdgpu_precision_string == "int8")
        return AMDGPU_PRECISION_INT8;
    if (amdgpu_precision_string == "uint8")
        return AMDGPU_PRECISION_UINT8;
    if (amdgpu_precision_string == "int4")
        return AMDGPU_PRECISION_INT4;
    if (amdgpu_precision_string == "uint4")
        return AMDGPU_PRECISION_UINT4;
    if (amdgpu_precision_string == "int16")
        return AMDGPU_PRECISION_INT16;
    if (amdgpu_precision_string == "uint16")
        return AMDGPU_PRECISION_UINT16;
    return AMDGPU_PRECISION_UNKNOWN;
}

float amdgpu_precision_data_byte(const std::string& amdgpu_precision_str){
    amdgpu_precision_t amdgpu_precision = amdgpu_string_to_precision(amdgpu_precision_str);
    if(amdgpu_precision == AMDGPU_PRECISION_FP32)
        return 4;
    if(amdgpu_precision == AMDGPU_PRECISION_FP16 || amdgpu_precision == AMDGPU_PRECISION_BF16 )
        return 2;
    if(amdgpu_precision == AMDGPU_PRECISION_INT16 || amdgpu_precision == AMDGPU_PRECISION_UINT16 )
        return 2;
    if(amdgpu_precision == AMDGPU_PRECISION_INT8 || amdgpu_precision == AMDGPU_PRECISION_UINT8)
        return 1;
    if(amdgpu_precision == AMDGPU_PRECISION_INT4 || amdgpu_precision == AMDGPU_PRECISION_INT4)
        return 0.5;
}

int amdgpu_wave_size(const amdgpu_arch_t &amdgpu_arch){
    if( amdgpu_arch < 1000)
        return 64;
    else
        return 32;
}

int amdgpu_sgpr_limit(const amdgpu_arch_t &amdgpu_arch){
    // https://llvm.org/docs/AMDGPUOperandSyntax.html
    if(amdgpu_arch <1000)
        return 102;
    else
        return 106;
}

/* IDLE defined but never used */
struct amdgpu_arch_detail_t{
    float theoretical_fp32_gflops(){
        return num_cu * simd_per_cu * (sclk_mhz / 1000.0) * fp32_fma_per_cycle;
    }

    float theoretical_bandwidth_gbps(){
        return (mclk_mhz / 1000.0) * (memory_bus_width_bits / 8) * memory_op_per_cycle;
    }

    amdgpu_arch_t arch = AMDGPU_ARCH_UNKNOWN;
    int num_cu         = 0;
    int simd_per_cu    = 0;
    int sclk_mhz       = 0;
    int mclk_mhz       = 0;
    int lds_size       = 0; // Byte
    int lds_banks      = 0;
    int l1_size        = 0;
    int l1_cache_line  = 0;
    int l2_size        = 0;
    int l2_cache_line  = 0;
    int mem_channels   = 0;
    int vgpr_per_cu    = 0;
    int sgpr_per_cu    = 0;
    int agpr_per_cu    = 0;
    int wavefront_size = 64;
    int max_waves_per_cu       = 0;
    int fp32_fma_per_cycle     = 0;
    int memory_op_per_cycle    = 0; // Read Write
    int memory_bus_width_bits  = 0;
};

/* IDLE defined but never used */
int amdgpu_calculate_occupancy(const amdgpu_arch_detail_t& arch_detail,
                               const int& vgpr_per_thread,
                               const int& block_size,
                               const int& lds_per_block){
    int vgpr_per_block = vgpr_per_thread * block_size;
    if(vgpr_per_block > arch_detail.vgpr_per_cu){
        printf("vgpr required: %d is larger than hw vgpr: %d \n", vgpr_per_block, arch_detail.vgpr_per_cu);
        return 0;
    }
    int block_consider_vgpr = arch_detail.vgpr_per_cu/vgpr_per_block;
    if(lds_per_block > arch_detail.lds_size){
        printf("LDS required: %d is larger than hw LDS: %d \n", lds_per_block, arch_detail.lds_size);
        return 0;
    }
    int blocks_consider_lds = arch_detail.lds_size/lds_per_block;
    return block_consider_vgpr > blocks_consider_lds ? blocks_consider_lds : block_consider_vgpr;
}

/* IDLE defined but never used */
bool amdgpu_valid_occupancy_with_max_waves(const amdgpu_arch_detail_t& arch_detail,
                                           const int& block_size,
                                           const int& occupancy){
    assert(block_size >= arch_detail.wavefront_size);
    assert(block_size % arch_detail.wavefront_size == 0);
    int waves_per_block = block_size / arch_detail.wavefront_size;
    return waves_per_block * occupancy <= arch_detail.max_waves_per_cu;
}

// def amdgpu_get_gfx906_60cu():
//     gfx906_60cu = amdgpu_arch_detail_t()
//     gfx906_60cu.arch            = AMDGPU_ARCH_GFX906
//     gfx906_60cu.num_cu          = 60
//     gfx906_60cu.simd_per_cu     = 64
//     gfx906_60cu.sclk_mhz        = 1725
//     gfx906_60cu.mclk_mhz        = 1000
//     gfx906_60cu.lds_size        = 65536
//     gfx906_60cu.lds_banks       = 32
//     gfx906_60cu.l1_size         = 16384
//     gfx906_60cu.l2_size         = 0
//     gfx906_60cu.mem_channels    = 0
//     gfx906_60cu.vgpr_per_cu     = 65536
//     gfx906_60cu.sgpr_per_cu     = 3200
//     gfx906_60cu.agpr_per_cu     = 0
//     gfx906_60cu.wavefront_size      = 64
//     gfx906_60cu.max_waves_per_cu    = 40
//     gfx906_60cu.fp32_fma_per_cycle  = 2
//     gfx906_60cu.memory_op_per_cycle = 2     # read write
//     gfx906_60cu.memory_bus_width_bits = 4096
//     return gfx906_60cu

struct amdgpu_arch_config_t{
    amdgpu_arch_config_t(const std::unordered_map<std::string, int> & arch_dict){
        _dict_with_default_t ad(arch_dict);
        arch = ad("arch", AMDGPU_ARCH_GFX906);
        if(arch == AMDGPU_ARCH_GFX900){
            use_dlops  = ad("use_dlops", false);
            use_xdlops = ad("use_xdlops", false);
        }
        if(arch == AMDGPU_ARCH_GFX906){
            use_dlops  = ad("use_dlops", true);
            use_xdlops = ad("use_xdlops", false);
        }
        else if(arch == AMDGPU_ARCH_GFX908 || arch == AMDGPU_ARCH_GFX90A){
            use_dlops  = ad("use_dlops", false);
            use_xdlops = ad("use_xdlops", true);
        }
        else if(arch == AMDGPU_ARCH_GFX1030){
            use_dlops  = ad("use_dlops", true);
            use_xdlops = ad("use_xdlops", false);
        }

        data_type      = ad("data_type", AMDGPU_PRECISION_FP32);
        code_object    = ad("code_object", AMDGPU_CODEOBJECT_V3);
    }

    int arch = AMDGPU_ARCH_UNKNOWN;
    bool use_dlops = false;
    bool use_xdlops = false;
    int data_type = AMDGPU_PRECISION_UNKNOWN;
    int code_object = AMDGPU_CODEOBJECT_UNKNOWN;
};

class amdgpu_kernel_code_t{
    amdgpu_kernel_code_t(const std::unordered_map<std::string, int> & kernel_code_dict){
        _dict_with_default_t kc(kernel_code_dict);
        enable_sgpr_private_segment_buffer     = kc("enable_sgpr_private_segment_buffer", 0);
        enable_sgpr_dispatch_ptr               = kc("enable_sgpr_dispatch_ptr", 0);
        enable_sgpr_queue_ptr                  = kc("enable_sgpr_queue_ptr", 0);
        enable_sgpr_kernarg_segment_ptr        = kc("enable_sgpr_kernarg_segment_ptr", 1);
        enable_sgpr_dispatch_id                = kc("enable_sgpr_dispatch_id", 0);
        // other sgpr related to be implemented
        user_sgpr_count                        = cal_user_sgpr_count();

        enable_sgpr_workgroup_id_x             = kc("enable_sgpr_workgroup_id_x", 1);
        enable_sgpr_workgroup_id_y             = kc("enable_sgpr_workgroup_id_y", 0);
        enable_sgpr_workgroup_id_z             = kc("enable_sgpr_workgroup_id_z", 0);
        enable_vgpr_workitem_id                = kc("enable_vgpr_workitem_id", 0);

        float_mode                             = kc("float_mode", 192);

        is_ptr64                               = kc("is_ptr64", 1);
        workgroup_group_segment_byte_size      = kc("workgroup_group_segment_byte_size", 0);
        wavefront_sgpr_count                   = kc("wavefront_sgpr_count", 0);
        workitem_vgpr_count                    = kc("workitem_vgpr_count", 0);

        // FIXME string type workitem_vgpr_count
        // if type(workitem_vgpr_count) is str and workitem_vgpr_count == "v_end":
        //     granulated_workitem_vgpr_count     = "(v_end-1)/4"
        // VCC, FLAT_SCRATCH and XNACK must be counted
        // if type(wavefront_sgpr_count) is str and wavefront_sgpr_count == "s_end+2*3":
        //     granulated_wavefront_sgpr_count    = "(s_end+2*3-1)/8"
        granulated_workitem_vgpr_count     = cal_granulated_workitem_vgpr_count();
        granulated_wavefront_sgpr_count    = cal_granulated_wavefront_sgpr_count();
        kernarg_segment_byte_size              = kc("kernarg_segment_byte_size", 0);
        tg_split                               = kc("tg_split", 0);
        accum_offset                           = kc("accum_offset", 0);
        wavefront_size                         = kc("wavefront_size", 64);
        cumode                                 = kc("cumode", 1);   // 0-cu mode, 1-wgp mode. for gfx>10
    }

    int cal_user_sgpr_count(){
        int count = 0;
        if (enable_sgpr_private_segment_buffer)
            count += 4;
        if (enable_sgpr_dispatch_ptr)
            count += 2;
        if (enable_sgpr_queue_ptr)
            count += 2;
        if (enable_sgpr_kernarg_segment_ptr)
            count += 2;
        if (enable_sgpr_dispatch_id)
            count += 2;
        return count;
    }

    int cal_granulated_workitem_vgpr_count(){
        return (workitem_vgpr_count -1) / 4;
    }

    int cal_granulated_wavefront_sgpr_count(){
        return (wavefront_sgpr_count -8) / 4;
    }

    int enable_sgpr_private_segment_buffer;
    int enable_sgpr_dispatch_ptr;
    int enable_sgpr_queue_ptr;
    int enable_sgpr_kernarg_segment_ptr;
    int enable_sgpr_dispatch_id;
    int user_sgpr_count;
    int enable_sgpr_workgroup_id_x;
    int enable_sgpr_workgroup_id_y;
    int enable_sgpr_workgroup_id_z;
    int enable_vgpr_workitem_id;
    int float_mode;
    int is_ptr64;                         
    int workgroup_group_segment_byte_size;
    int wavefront_sgpr_count;             
    int workitem_vgpr_count;
    int granulated_workitem_vgpr_count;              
    int granulated_wavefront_sgpr_count;
    int kernarg_segment_byte_size;
    int tg_split;
    int accum_offset;
    int wavefront_size;
    int cumode;
};

/*
class amdgpu_kernel_arg_t(object):
    '''
    http://llvm.org/docs/AMDGPUUsage.html#code-object-v3-metadata-mattr-code-object-v3
    '''
    def __init__(self, name, size, offset, value_kind, value_type, **misc):
        self.name = name
        self.size = size
        self.offset = offset
        self.value_kind = value_kind
        self.value_type = value_type
        self.misc = misc
    def serialize_as_metadata(self):
        misc_metadata = ''
        if self.value_kind == 'global_buffer':
            assert self.misc
            misc_metadata += ', .address_space: {}'.format(self.misc['address_space'])
            misc_metadata += ', .is_const: {}'.format(self.misc['is_const'])
        return '    - {{ .name: {:<10}, .size: {}, .offset: {:>3}, .value_kind: {}, .value_type: {}{}}}'.format(
            self.name, self.size, self.offset, self.value_kind, self.value_type, misc_metadata)

class amdgpu_kernel_info_t(object):
    def __init__(self, kernel_code, kernel_name, kernel_block_size, kernel_args):
        self.kernel_code = kernel_code
        self.kernel_name = kernel_name
        self.kernel_block_size = kernel_block_size
        self.kernel_args = kernel_args

class amd_kernel_code_t(mc_base_t):
    def __init__(self, mc, kernel_info):
        mc_base_t.__init__(self, mc)
        self.ki = kernel_info

    def emit(self):
        if self.mc.arch_config.code_object == AMDGPU_CODEOBJECT_V2:
            self._emit('.amd_kernel_code_t')
            with self._indent_context():
                if self.ki.kernel_code.enable_sgpr_private_segment_buffer:
                    self._emit('enable_sgpr_private_segment_buffer = {}'.format(    self.ki.kernel_code.enable_sgpr_private_segment_buffer))
                if self.ki.kernel_code.enable_sgpr_dispatch_ptr:
                    self._emit('enable_sgpr_dispatch_ptr = {}'.format(              self.ki.kernel_code.enable_sgpr_dispatch_ptr))
                if self.ki.kernel_code.enable_sgpr_queue_ptr:
                    self._emit('enable_sgpr_queue_ptr = {}'.format(                 self.ki.kernel_code.enable_sgpr_queue_ptr))
                self._emit('enable_sgpr_kernarg_segment_ptr = {}'.format(           self.ki.kernel_code.enable_sgpr_kernarg_segment_ptr))
                if self.ki.kernel_code.enable_sgpr_dispatch_id:
                    self._emit('enable_sgpr_dispatch_id'.format(                    self.ki.kernel_code.enable_sgpr_dispatch_id))
                # other sgpr related to be implemented 
                self._emit('user_sgpr_count = {}'.format(                           self.ki.kernel_code.user_sgpr_count))
                if self.ki.kernel_code.enable_sgpr_workgroup_id_x:
                    self._emit('enable_sgpr_workgroup_id_x = {}'.format(            self.ki.kernel_code.enable_sgpr_workgroup_id_x))
                if self.ki.kernel_code.enable_sgpr_workgroup_id_y:
                    self._emit('enable_sgpr_workgroup_id_y = {}'.format(            self.ki.kernel_code.enable_sgpr_workgroup_id_y))
                if self.ki.kernel_code.enable_sgpr_workgroup_id_z:
                    self._emit('enable_sgpr_workgroup_id_z = {}'.format(            self.ki.kernel_code.enable_sgpr_workgroup_id_z))
                self._emit('enable_vgpr_workitem_id = {}'.format(                   self.ki.kernel_code.enable_vgpr_workitem_id))
                self._emit('is_ptr64 = {}'.format(                                  self.ki.kernel_code.is_ptr64))
                self._emit('float_mode = {}'.format(                                self.ki.kernel_code.float_mode))
                self._emit('workgroup_group_segment_byte_size = {}'.format(         self.ki.kernel_code.workgroup_group_segment_byte_size))
                self._emit('kernarg_segment_byte_size = {}'.format(                 self.ki.kernel_code.kernarg_segment_byte_size))
                self._emit('wavefront_sgpr_count = {}'.format(                      self.ki.kernel_code.wavefront_sgpr_count))
                self._emit('workitem_vgpr_count = {}'.format(                       self.ki.kernel_code.workitem_vgpr_count))
                self._emit('granulated_workitem_vgpr_count = {}'.format(            self.ki.kernel_code.granulated_workitem_vgpr_count))
                self._emit('granulated_wavefront_sgpr_count = {}'.format(           self.ki.kernel_code.granulated_wavefront_sgpr_count))
            self._emit('.end_amd_kernel_code_t')
        elif self.mc.arch_config.code_object == AMDGPU_CODEOBJECT_V3:
            self._emit('.rodata')   # v3 is 64 byte rodata for each kerenl
            self._emit('.p2align 6')
            self._emit('.amdhsa_kernel {}'.format(self.ki.kernel_name))
            with self._indent_context():
                if self.ki.kernel_code.kernarg_segment_byte_size > 0:
                    self._emit('.amdhsa_group_segment_fixed_size {}'.format(        self.ki.kernel_code.workgroup_group_segment_byte_size))
                if self.ki.kernel_code.enable_sgpr_dispatch_ptr:
                    self._emit('.amdhsa_user_sgpr_dispatch_ptr {}'.format(          self.ki.kernel_code.enable_sgpr_dispatch_ptr))

                self._emit('.amdhsa_user_sgpr_kernarg_segment_ptr {}'.format(       self.ki.kernel_code.enable_sgpr_kernarg_segment_ptr))

                if self.ki.kernel_code.enable_sgpr_workgroup_id_x:
                    self._emit('.amdhsa_system_sgpr_workgroup_id_x {}'.format(      self.ki.kernel_code.enable_sgpr_workgroup_id_x))
                if self.ki.kernel_code.enable_sgpr_workgroup_id_y:
                    self._emit('.amdhsa_system_sgpr_workgroup_id_y {}'.format(      self.ki.kernel_code.enable_sgpr_workgroup_id_y))
                if self.ki.kernel_code.enable_sgpr_workgroup_id_z:
                    self._emit('.amdhsa_system_sgpr_workgroup_id_z {}'.format(      self.ki.kernel_code.enable_sgpr_workgroup_id_z))

                self._emit('.amdhsa_system_vgpr_workitem_id {}'.format(             self.ki.kernel_code.enable_vgpr_workitem_id))
                self._emit('.amdhsa_next_free_vgpr {}'.format(                      self.ki.kernel_code.workitem_vgpr_count))
                self._emit('.amdhsa_next_free_sgpr {}'.format(                      self.ki.kernel_code.wavefront_sgpr_count - 2*3))

                self._emit('.amdhsa_ieee_mode 0')   # seems everyone close this?
                self._emit('.amdhsa_dx10_clamp 0')  # seems everyone close this?
                if self.mc.arch_config.arch == AMDGPU_ARCH_GFX90A:
                    self._emit('.amdhsa_tg_split {}'.format(                        self.ki.kernel_code.tg_split))
                    self._emit('.amdhsa_accum_offset {}'.format(                    self.ki.kernel_code.accum_offset))
                if self.mc.arch_config.arch >= 1000:
                    self._emit('.amdhsa_wavefront_size32 {}'.format(                1 if self.ki.kernel_code.wavefront_size == 32 else 0))
                    self._emit('.amdhsa_workgroup_processor_mode {}'.format(        1 if not self.ki.kernel_code.cumode else 0))
            self._emit('.end_amdhsa_kernel')
        else:
            assert False

class amdgpu_metadata_t(mc_base_t):
    '''
    only implement in cov3
    '''
    def __init__(self, mc, kernel_info):
        mc_base_t.__init__(self, mc)
        self.ki = kernel_info

    def emit_one_kernel_metadata(self, ki_):
        self._emit('  - .name: {}'.format(                          ki_.kernel_name))
        self._emit('    .symbol: {}.kd'.format(                     ki_.kernel_name))
        self._emit('    .sgpr_count: {}'.format(                    ki_.kernel_code.wavefront_sgpr_count))
        self._emit('    .vgpr_count: {}'.format(                    ki_.kernel_code.workitem_vgpr_count))
        self._emit('    .kernarg_segment_align: {}'.format(         8))     # default set to 8
        self._emit('    .kernarg_segment_size: {}'.format(          ki_.kernel_code.kernarg_segment_byte_size))
        self._emit('    .group_segment_fixed_size: {}'.format(      ki_.kernel_code.workgroup_group_segment_byte_size))
        self._emit('    .private_segment_fixed_size: {}'.format(    0))     # hard code to 0
        self._emit('    .wavefront_size: {}'.format(                ki_.kernel_code.wavefront_size))
        self._emit('    .reqd_workgroup_size : [{}]'.format(        '{}, 1, 1'.format( ki_.kernel_block_size) \
                                                                                if type(ki_.kernel_block_size) is int else \
                                                                    '{}, {}, {}'.format(ki_.kernel_block_size[0],
                                                                                ki_.kernel_block_size[1],ki_.kernel_block_size[2])))
        self._emit('    .max_flat_workgroup_size: {}'.format(       ki_.kernel_block_size if type(ki_.kernel_block_size) is int else \
                                                                    ki_.kernel_block_size[0]*ki_.kernel_block_size[1]*ki_.kernel_block_size[2]))
        self._emit('    .args:')
        assert ki_.kernel_args
        for kern_arg in ki_.kernel_args:
            self._emit(kern_arg.serialize_as_metadata())

    def emit(self):
        if self.mc.arch_config.code_object == AMDGPU_CODEOBJECT_V3:
            self._emit('.amdgpu_metadata')
            self._emit('---')
            self._emit('amdhsa.version: [ 1, 0 ]')
            self._emit('amdhsa.kernels:')
            if type(self.ki) is list:
                for k in self.ki:
                    self.emit_one_kernel_metadata(k)
            else:
                self.emit_one_kernel_metadata(self.ki)
            self._emit('...')
            self._emit('.end_amdgpu_metadata')

class hsa_header_t(mc_base_t):
    '''
    only used in cov2
    '''
    def __init__(self, mc):
        mc_base_t.__init__(self, mc)
    def emit(self):
        if self.mc.arch_config.code_object == AMDGPU_CODEOBJECT_V2:
            self._emit(".hsa_code_object_version 2,1")
            self._emit(".hsa_code_object_isa")
            self._emit_empty_line()

class hsa_footer_t(mc_base_t):
    def __init__(self, mc):
        mc_base_t.__init__(self, mc)
    def emit(self):
        pass
*/