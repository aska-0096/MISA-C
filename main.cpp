#include <iostream>
#include <cstdio>
#include <string>

// #include "utility/inputflag_parser.hpp"
// #include "src/config_parser.h"
#include "src/args.h"
#include "src/igemm_gtc_base.h"

int main(int argc, char* argv[])
{
    /* Command line input flags parser */
    args_t inputflag = create_inputflags(argc, argv);

    /* File parser */
    std::string config_file = parser->get_inputmap()['i'].value;
    config_parser_t config_parser(config_file);
    /* Raw file
     * Integrated configuration
     * Splited tunable list, lowest granularity
     */
    auto unexpanded_content = config_parser.parse();
    auto content = igemm_try_expand_tunable_content(unexpanded_content);
    auto tunables = igemm_gtc_tunable_from_config(content);
    /* TODO Feed which level data to conv_driver.exe?
     * CodegenDriver need [content]
     * ConvDriver need [config_file]
     */
    auto arch = content.get_section("codegen").at("arch");
    auto code_object = content.get_section("codegen").at("code_object");
    bool has_fp16_config = igemm_check_fp16_configs(tunables);
    bool has_int8_config = igemm_check_int8_configs(tunables);
    bool has_bf16_config = igemm_check_bf16_configs(tunables);
    bool has_int4_config = igemm_check_int4_configs(tunables);


    /* Feed parsed arguments and tunables to asm code generator
     * MisaPy: Codegen and Kerenl Launcher run separately
     * MisaC: Code generation, compilation and execution on the fly
     */


    /* Codegen + Compilation
     * Return HSACO
     */


    /* Kernel Launcher
     * Accept HSACO and execute it, reusable
     */

    


}