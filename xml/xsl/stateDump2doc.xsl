<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"><!--This converts a Moleculizer state dump into a tree-like HTML display in which each element and attribute name is a link to documentation via cgi.--><xsl:variable name="server-url"><xsl:value-of select="document('../mzr-defaults.xml')/moleculizer-defaults/server/@url"/></xsl:variable><xsl:variable name="path-doc-url" select="concat($server-url, '/cgi-bin/mzr-dispatch?task=mzr-doc-query&amp;schema=mzrState.rng&amp;xpth=')"/><xsl:variable name="caption" select="'Moleculizer State Dump'"/><xsl:template match="*"><xsl:param name="xpth"/><xsl:variable name="curpth" select="concat($xpth, name())"/><xsl:element name="li"><xsl:element name="a"><xsl:attribute name="href"><xsl:value-of select="concat($path-doc-url, $curpth)"/></xsl:attribute><xsl:element name="strong"><xsl:value-of select="name()"/></xsl:element></xsl:element><xsl:apply-templates mode="consistency" select="."><xsl:with-param name="xpth" select="$curpth"/></xsl:apply-templates><xsl:if test="@*"><xsl:element name="table"><xsl:attribute name="border"><xsl:text>1</xsl:text></xsl:attribute><xsl:attribute name="cellpadding"><xsl:text>2</xsl:text></xsl:attribute><xsl:for-each select="@*"><xsl:element name="tr"><xsl:attribute name="align"><xsl:text>center</xsl:text></xsl:attribute><xsl:element name="td"><xsl:element name="a"><xsl:attribute name="href"><xsl:value-of select="concat($path-doc-url, $curpth, '/@', name())"/></xsl:attribute><xsl:value-of select="name()"/></xsl:element></xsl:element><xsl:element name="td"><xsl:value-of select="."/></xsl:element></xsl:element></xsl:for-each></xsl:element></xsl:if><xsl:if test="*"><xsl:element name="ul"><xsl:apply-templates><xsl:with-param name="xpth" select="concat($curpth, '/')"/></xsl:apply-templates></xsl:element></xsl:if></xsl:element></xsl:template><xsl:template match="/"><xsl:message>server-url is   <xsl:value-of select="$server-url"/></xsl:message><xsl:element name="head"><xsl:element name="title"><xsl:value-of select="$caption"/></xsl:element></xsl:element><xsl:element name="body"><xsl:element name="H1"><xsl:value-of select="$caption"/></xsl:element><xsl:element name="ul"><xsl:apply-templates><xsl:with-param name="xpth" select="'/'"/></xsl:apply-templates></xsl:element></xsl:element></xsl:template></xsl:stylesheet>