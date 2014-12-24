/*
 * balde: A microframework for C based on GLib and bad intentions.
 * Copyright (C) 2013-2014 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the LGPL-2 License.
 * See the file COPYING.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <glib.h>
#include "../src/template/template.h"
#include "../src/template/parser.h"
#include "utils.h"


void
balde_assert_template_import(GSList *l, const gchar *import)
{
    balde_template_block_t *node = l->data;
    g_assert(node->type == BALDE_TEMPLATE_IMPORT_BLOCK);
    balde_template_import_block_t *block = node->block;
    g_assert_cmpstr(block->import, ==, import);
}


void
balde_assert_template_include(GSList *l, const gchar *include)
{
    balde_template_block_t *node = l->data;
    g_assert(node->type == BALDE_TEMPLATE_INCLUDE_BLOCK);
    balde_template_include_block_t *block = node->block;
    g_assert_cmpstr(block->include, ==, include);
}


void
balde_assert_template_for(GSList *l, const gchar *item, const gchar *items)
{
    balde_template_block_t *node = l->data;
    g_assert(node->type == BALDE_TEMPLATE_FOR_BLOCK);
    balde_template_for_block_t *block = node->block;
    g_assert_cmpstr(block->item, ==, item);
    g_assert_cmpstr(block->items, ==, items);
}


void
balde_assert_template_for_end(GSList *l)
{
    balde_template_block_t *node = l->data;
    g_assert(node->type == BALDE_TEMPLATE_FOR_END_BLOCK);
    g_assert(node->block == NULL);
}


void
balde_assert_template_content(GSList *l, const gchar *content)
{
    balde_template_block_t *node = l->data;
    g_assert(node->type == BALDE_TEMPLATE_CONTENT_BLOCK);
    balde_template_content_block_t *block = node->block;
    g_assert_cmpstr(block->content, ==, content);
}


void
balde_assert_template_print_var(GSList *l, const gchar *variable)
{
    balde_template_block_t *node = l->data;
    g_assert(node->type == BALDE_TEMPLATE_PRINT_VAR_BLOCK);
    balde_template_print_var_block_t *block = node->block;
    g_assert_cmpstr(block->variable, ==, variable);
}


void
balde_assert_template_print_function_call(GSList *l, const gchar *name, ...)
{
    va_list args;
    va_start(args, name);
    balde_template_block_t *node = l->data;
    g_assert(node->type == BALDE_TEMPLATE_PRINT_FN_CALL_BLOCK);
    balde_template_print_fn_call_block_t *block = node->block;
    g_assert_cmpstr(block->name, ==, name);
    for (GSList *tmp = block->args; tmp != NULL; tmp = g_slist_next(tmp))
        g_assert_cmpstr(((balde_template_fn_arg_t*) tmp->data)->content, ==,
            va_arg(args, const gchar*));
    va_end(args);
}


void
balde_assert_template_equal(const gchar *v1, const gchar *v2)
{
    if (g_strcmp0(v1, v2) != 0) {
        g_print("\n==================================================================\n");
        g_print("%s\n", v1);
        g_print("==================================================================\n");
        g_print("%s\n", v2);
        g_print("==================================================================\n");
        g_assert(FALSE);
    }
}


void
test_template_generate_source_without_vars(void)
{
    gchar *tmpl = get_template("foo.html");
    gchar *rv = balde_template_generate_source("bola", tmpl);
    g_free(tmpl);
    balde_assert_template_equal(rv,
        "// WARNING: this file was generated automatically by balde-template-gen\n"
        "\n"
        "#include <balde.h>\n"
        "#include <glib.h>\n"
        "\n"
        "static const gchar *balde_template_bola_format = \"ads qwe\\n\";\n"
        "extern void balde_template_bola(balde_app_t *app, balde_request_t *request, "
        "balde_response_t *response);\n"
        "\n"
        "void\n"
        "balde_template_bola(balde_app_t *app, balde_request_t *request, "
        "balde_response_t *response)\n"
        "{\n"
        "    gchar *rv = g_strdup(balde_template_bola_format);\n"
        "    balde_response_append_body(response, rv);\n"
        "    g_free(rv);\n"
        "}\n");
    g_free(rv);
}


void
test_template_generate_source_single_var(void)
{
    gchar *tmpl = get_template("foo2.html");
    gchar *rv = balde_template_generate_source("bola", tmpl);
    g_free(tmpl);
    balde_assert_template_equal(rv,
        "// WARNING: this file was generated automatically by balde-template-gen\n"
        "\n"
        "#include <balde.h>\n"
        "#include <glib.h>\n"
        "\n"
        "static const gchar *balde_template_bola_format = \"ads %s qwe\\n\";\n"
        "extern void balde_template_bola(balde_app_t *app, balde_request_t *request, "
        "balde_response_t *response);\n"
        "\n"
        "void\n"
        "balde_template_bola(balde_app_t *app, balde_request_t *request, "
        "balde_response_t *response)\n"
        "{\n"
        "    gchar *rv = g_strdup_printf(balde_template_bola_format, balde_response_get_tmpl_var(response, \"guda\"));\n"
        "    balde_response_append_body(response, rv);\n"
        "    g_free(rv);\n"
        "}\n");
    g_free(rv);
}


void
test_template_generate_source_single_var_with_imports(void)
{
    gchar *tmpl = get_template("foo3.html");
    gchar *rv = balde_template_generate_source("bola", tmpl);
    g_free(tmpl);
    balde_assert_template_equal(rv,
        "// WARNING: this file was generated automatically by balde-template-gen\n"
        "\n"
        "#include <balde.h>\n"
        "#include <glib.h>\n"
        "#include <bola.h>\n"
        "#include <bola/guda.h>\n"
        "\n"
        "static const gchar *balde_template_bola_format = \"ads %s qwe\\n\\n         \\n\\n\";\n"
        "extern void balde_template_bola(balde_app_t *app, balde_request_t *request, "
        "balde_response_t *response);\n"
        "\n"
        "void\n"
        "balde_template_bola(balde_app_t *app, balde_request_t *request, "
        "balde_response_t *response)\n"
        "{\n"
        "    gchar *rv = g_strdup_printf(balde_template_bola_format, balde_response_get_tmpl_var(response, \"guda\"));\n"
        "    balde_response_append_body(response, rv);\n"
        "    g_free(rv);\n"
        "}\n");
    g_free(rv);
}


void
test_template_generate_source_single_var_with_single_quoted_imports(void)
{
    gchar *tmpl = get_template("foo4.html");
    gchar *rv = balde_template_generate_source("bola", tmpl);
    g_free(tmpl);
    balde_assert_template_equal(rv,
        "// WARNING: this file was generated automatically by balde-template-gen\n"
        "\n"
        "#include <balde.h>\n"
        "#include <glib.h>\n"
        "#include <bola.h>\n"
        "#include <bola/guda.h>\n"
        "\n"
        "static const gchar *balde_template_bola_format = \"ads %s qwe\\n\\n         \\n\\n\";\n"
        "extern void balde_template_bola(balde_app_t *app, balde_request_t *request, "
        "balde_response_t *response);\n"
        "\n"
        "void\n"
        "balde_template_bola(balde_app_t *app, balde_request_t *request, "
        "balde_response_t *response)\n"
        "{\n"
        "    gchar *rv = g_strdup_printf(balde_template_bola_format, balde_response_get_tmpl_var(response, \"guda\"));\n"
        "    balde_response_append_body(response, rv);\n"
        "    g_free(rv);\n"
        "}\n");
    g_free(rv);
}


void
test_template_generate_source_multiple_vars(void)
{
    gchar *tmpl = get_template("foo5.html");
    gchar *rv = balde_template_generate_source("bola", tmpl);
    g_free(tmpl);
    balde_assert_template_equal(rv,
        "// WARNING: this file was generated automatically by balde-template-gen\n"
        "\n"
        "#include <balde.h>\n"
        "#include <glib.h>\n"
        "\n"
        "static const gchar *balde_template_bola_format = \"ads %s %s %s qwe\\n\";\n"
        "extern void balde_template_bola(balde_app_t *app, balde_request_t *request, "
        "balde_response_t *response);\n"
        "\n"
        "void\n"
        "balde_template_bola(balde_app_t *app, balde_request_t *request, "
        "balde_response_t *response)\n"
        "{\n"
        "    gchar *rv = g_strdup_printf(balde_template_bola_format, balde_response_get_tmpl_var(response, \"guda\"), balde_response_get_tmpl_var(response, \"bola\"), balde_response_get_tmpl_var(response, \"balde\"));\n"
        "    balde_response_append_body(response, rv);\n"
        "    g_free(rv);\n"
        "}\n");
    g_free(rv);
}


void
test_template_generate_source_multiple_vars_and_lines(void)
{
    gchar *tmpl = get_template("foo6.html");
    gchar *rv = balde_template_generate_source("bola", tmpl);
    g_free(tmpl);
    balde_assert_template_equal(rv,
        "// WARNING: this file was generated automatically by balde-template-gen\n"
        "\n"
        "#include <balde.h>\n"
        "#include <glib.h>\n"
        "\n"
        "static const gchar *balde_template_bola_format = \"ads %s\\n\\n%s\\n%s qwe\\n\";\n"
        "extern void balde_template_bola(balde_app_t *app, balde_request_t *request, "
        "balde_response_t *response);\n"
        "\n"
        "void\n"
        "balde_template_bola(balde_app_t *app, balde_request_t *request, "
        "balde_response_t *response)\n"
        "{\n"
        "    gchar *rv = g_strdup_printf(balde_template_bola_format, balde_response_get_tmpl_var(response, \"guda\"), balde_response_get_tmpl_var(response, \"bola\"), balde_response_get_tmpl_var(response, \"balde\"));\n"
        "    balde_response_append_body(response, rv);\n"
        "    g_free(rv);\n"
        "}\n");
    g_free(rv);
}


void
test_template_generate_source_multiple_vars_and_lines_with_function_calls(void)
{
    gchar *tmpl = get_template("foo7.html");
    gchar *rv = balde_template_generate_source("bola", tmpl);
    g_free(tmpl);
    balde_assert_template_equal(rv,
        "// WARNING: this file was generated automatically by balde-template-gen\n"
        "\n"
        "#include <balde.h>\n"
        "#include <glib.h>\n"
        "\n"
        "static const gchar *balde_template_bola_format = \"ads %s\\n\\n%s\\n\\n%s %s\\n%s qwe\\n\";\n"
        "extern void balde_template_bola(balde_app_t *app, balde_request_t *request, "
        "balde_response_t *response);\n"
        "\n"
        "void\n"
        "balde_template_bola(balde_app_t *app, balde_request_t *request, "
        "balde_response_t *response)\n"
        "{\n"
        "    gchar *a1 = balde_tmpl_bola(app, request);\n"
        "    gchar *a2 = balde_tmpl_chunda(app, request, balde_response_get_tmpl_var(response, \"quico\"), \"cola\", -2, 1.5, \"chaves\", TRUE, balde_response_get_tmpl_var(response, \"popis\"), NULL);\n"
        "    gchar *a3 = balde_tmpl_asd(app, request, \"XD\");\n"
        "    gchar *rv = g_strdup_printf(balde_template_bola_format, balde_response_get_tmpl_var(response, \"guda\"), a1, a2, a3, balde_response_get_tmpl_var(response, \"balde\"));\n"
        "    balde_response_append_body(response, rv);\n"
        "    g_free(rv);\n"
        "    g_free(a3);\n"
        "    g_free(a2);\n"
        "    g_free(a1);\n"
        "}\n");
    g_free(rv);
}


void
test_template_generate_source_multiple_vars_and_lines_with_function_calls_and_single_quotes(void)
{
    gchar *tmpl = get_template("foo8.html");
    gchar *rv = balde_template_generate_source("bola", tmpl);
    g_free(tmpl);
    balde_assert_template_equal(rv,
        "// WARNING: this file was generated automatically by balde-template-gen\n"
        "\n"
        "#include <balde.h>\n"
        "#include <glib.h>\n"
        "\n"
        "static const gchar *balde_template_bola_format = \"ads %s\\n\\n%s\\n\\n%s %s\\n%s qwe\\n\";\n"
        "extern void balde_template_bola(balde_app_t *app, balde_request_t *request, "
        "balde_response_t *response);\n"
        "\n"
        "void\n"
        "balde_template_bola(balde_app_t *app, balde_request_t *request, "
        "balde_response_t *response)\n"
        "{\n"
        "    gchar *a1 = balde_tmpl_bola(app, request);\n"
        "    gchar *a2 = balde_tmpl_chunda(app, request, balde_response_get_tmpl_var(response, \"quico\"), \"cola\", -2, 1.5, \"chaves\", TRUE, balde_response_get_tmpl_var(response, \"popis\"), \"bo'la\");\n"
        "    gchar *a3 = balde_tmpl_asd(app, request, \"XD\");\n"
        "    gchar *rv = g_strdup_printf(balde_template_bola_format, balde_response_get_tmpl_var(response, \"guda\"), a1, a2, a3, balde_response_get_tmpl_var(response, \"balde\"));\n"
        "    balde_response_append_body(response, rv);\n"
        "    g_free(rv);\n"
        "    g_free(a3);\n"
        "    g_free(a2);\n"
        "    g_free(a1);\n"
        "}\n");
    g_free(rv);
}


void
test_template_generate_source_with_percent(void)
{
    gchar *tmpl = get_template("foo9.html");
    gchar *rv = balde_template_generate_source("bola", tmpl);
    g_free(tmpl);
    balde_assert_template_equal(rv,
        "// WARNING: this file was generated automatically by balde-template-gen\n"
        "\n"
        "#include <balde.h>\n"
        "#include <glib.h>\n"
        "\n"
        "static const gchar *balde_template_bola_format = \"ad %%s %% %s qwe\\n\";\n"
        "extern void balde_template_bola(balde_app_t *app, balde_request_t *request, "
        "balde_response_t *response);\n"
        "\n"
        "void\n"
        "balde_template_bola(balde_app_t *app, balde_request_t *request, "
        "balde_response_t *response)\n"
        "{\n"
        "    gchar *rv = g_strdup_printf(balde_template_bola_format, balde_response_get_tmpl_var(response, \"guda\"));\n"
        "    balde_response_append_body(response, rv);\n"
        "    g_free(rv);\n"
        "}\n");
    g_free(rv);
}


void
test_template_generate_source_with_include(void)
{
    gchar *tmpl = get_template("base.html");
    gchar *rv = balde_template_generate_source("bola", tmpl);
    g_free(tmpl);
    balde_assert_template_equal(rv,
        "// WARNING: this file was generated automatically by balde-template-gen\n"
        "\n"
        "#include <balde.h>\n"
        "#include <glib.h>\n"
        "\n"
        "static const gchar *balde_template_bola_format = \"<html>\\n<head><title>%s</title></head>\\n<body>\\n<h1>%s</h1>\\n<h2>%s</h2>\\n<p>This page is: %s</p>\\n\\n</body>\\n</html>\\n\";\n"
        "extern void balde_template_bola(balde_app_t *app, balde_request_t *request, balde_response_t *response);\n"
        "\n"
        "void\n"
        "balde_template_bola(balde_app_t *app, balde_request_t *request, balde_response_t *response)\n"
        "{\n"
        "    gchar *rv = g_strdup_printf(balde_template_bola_format, balde_response_get_tmpl_var(response, \"title\"), balde_response_get_tmpl_var(response, \"title\"), balde_response_get_tmpl_var(response, \"subtitle\"), balde_response_get_tmpl_var(response, \"name\"));\n"
        "    balde_response_append_body(response, rv);\n"
        "    g_free(rv);\n"
        "}\n");
    g_free(rv);
}


void
test_template_generate_source_with_include_with_url_for(void)
{
    gchar *tmpl = get_template("base2.html");
    gchar *rv = balde_template_generate_source("bola", tmpl);
    g_free(tmpl);
    balde_assert_template_equal(rv,
        "// WARNING: this file was generated automatically by balde-template-gen\n"
        "\n"
        "#include <balde.h>\n"
        "#include <glib.h>\n"
        "\n"
        "static const gchar *balde_template_bola_format = \"<html>\\n<head><title>%s</title></head>\\n<body>\\n<h1>%s</h1>\\n<p>%s</p>\\n<h2>%s</h2>\\n<p>%s</p>\\n\\n<p>%s</p>\\n</body>\\n</html>\\n\";\n"
        "extern void balde_template_bola(balde_app_t *app, balde_request_t *request, balde_response_t *response);\n"
        "\n"
        "void\n"
        "balde_template_bola(balde_app_t *app, balde_request_t *request, balde_response_t *response)\n"
        "{\n"
        "    gchar *a1 = balde_tmpl_url_for(app, request, \"bola0\", FALSE, \"guda\");\n"
        "    gchar *a2 = balde_tmpl_url_for(app, request, \"bola\", FALSE, \"guda\");\n"
        "    gchar *a3 = balde_tmpl_url_for(app, request, \"bola2\", FALSE, \"guda\");\n"
        "    gchar *rv = g_strdup_printf(balde_template_bola_format, balde_response_get_tmpl_var(response, \"title\"), balde_response_get_tmpl_var(response, \"title\"), a1, balde_response_get_tmpl_var(response, \"subtitle\"), a2, a3);\n"
        "    balde_response_append_body(response, rv);\n"
        "    g_free(rv);\n"
        "    g_free(a3);\n"
        "    g_free(a2);\n"
        "    g_free(a1);\n"
        "}\n");
    g_free(rv);
}


void
test_template_generate_header(void)
{
    gchar *rv = balde_template_generate_header("bola");
    balde_assert_template_equal(rv,
        "// WARNING: this file was generated automatically by balde-template-gen\n"
        "\n"
        "#ifndef __bola_balde_template\n"
        "#define __bola_balde_template\n"
        "\n"
        "#include <balde.h>\n"
        "\n"
        "extern void balde_template_bola(balde_app_t *app, balde_request_t *request, "
        "balde_response_t *response);\n"
        "\n"
        "#endif\n");
    g_free(rv);
}


void
test_template_get_name(void)
{
    gchar *rv = balde_template_get_name("bola.guda.ação.html");
    g_assert_cmpstr(rv, ==, "bola_guda_a____o");
    g_free(rv);
}


void
test_template_parse(void)
{
    GSList *blocks = balde_template_parse(
        "Test\n"
        "\n"
        "   {% import \"bola.h\" %}\n"
        "foo {{ bola(gude, 'asd', \"zxc\", TRUE, NULL, 1.5, -2, \"chun\\\"da\", 'gu\\'da', FALSE, xd, +5, \"\") }}\n"
        "  {{ test }} \n"
        "    {% include 'foo.html' %}\n"
        "bola\n"
        "{% import 'asd.h' %}\n"
        "{% for chunda in guda %}\n"
        "uhuuuu\n"
        "{{ chunda }}\n"
        "{% endfor %}");
    g_assert(blocks != NULL);
    balde_assert_template_content(blocks, "Test\n\n   ");
    balde_assert_template_import(blocks->next, "bola.h");
    balde_assert_template_content(blocks->next->next, "\nfoo ");
    balde_assert_template_print_function_call(blocks->next->next->next, "bola",
        "gude", "\"asd\"", "\"zxc\"", "TRUE", "NULL", "1.5", "-2",
        "\"chun\\\"da\"", "\"gu'da\"", "FALSE", "xd", "+5", "\"\"");
    balde_assert_template_content(blocks->next->next->next->next, "\n  ");
    balde_assert_template_print_var(blocks->next->next->next->next->next, "test");
    balde_assert_template_content(blocks->next->next->next->next->next->next,
        " \n    ");
    balde_assert_template_include(
        blocks->next->next->next->next->next->next->next, "foo.html");
    balde_assert_template_content(
        blocks->next->next->next->next->next->next->next->next, "\nbola\n");
    balde_assert_template_import(
        blocks->next->next->next->next->next->next->next->next->next, "asd.h");
    balde_assert_template_content(
        blocks->next->next->next->next->next->next->next->next->next->next, "\n");
    balde_assert_template_for(
        blocks->next->next->next->next->next->next->next->next->next->next->next,
        "chunda", "guda");
    balde_assert_template_content(
        blocks->next->next->next->next->next->next->next->next->next->next->next->next,
        "\nuhuuuu\n");
    balde_assert_template_print_var(
        blocks->next->next->next->next->next->next->next->next->next->next->next->next->next,
        "chunda");
    balde_assert_template_content(
        blocks->next->next->next->next->next->next->next->next->next->next->next->next->next->next,
        "\n");
    balde_assert_template_for_end(
        blocks->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next);
    g_assert(blocks->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next == NULL);
    balde_template_free_blocks(blocks);
}


int
main(int argc, char** argv)
{
    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/template/generate_source_without_vars",
        test_template_generate_source_without_vars);
    g_test_add_func("/template/generate_source_single_var",
        test_template_generate_source_single_var);
    g_test_add_func("/template/generate_source_single_var_with_imports",
        test_template_generate_source_single_var_with_imports);
    g_test_add_func("/template/generate_source_single_var_with_single_quoted_imports",
        test_template_generate_source_single_var_with_single_quoted_imports);
    g_test_add_func("/template/generate_source_multiple_vars",
        test_template_generate_source_multiple_vars);
    g_test_add_func("/template/generate_source_multiple_vars_and_lines",
        test_template_generate_source_multiple_vars_and_lines);
    g_test_add_func("/template/generate_source_multiple_vars_and_lines_with_function_calls",
        test_template_generate_source_multiple_vars_and_lines_with_function_calls);
    g_test_add_func("/template/generate_source_multiple_vars_and_lines_with_function_calls_and_single_quotes",
        test_template_generate_source_multiple_vars_and_lines_with_function_calls_and_single_quotes);
    g_test_add_func("/template/generate_source_with_percent",
        test_template_generate_source_with_percent);
    g_test_add_func("/template/generate_source_with_include",
        test_template_generate_source_with_include);
    g_test_add_func("/template/generate_source_with_include_with_url_for",
        test_template_generate_source_with_include_with_url_for);
    g_test_add_func("/template/generate_header",
        test_template_generate_header);
    g_test_add_func("/template/get_name", test_template_get_name);
    g_test_add_func("/template/parse", test_template_parse);
    return g_test_run();
}
