<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"><xsl:output encoding="ISO-8859-1" method="html"/><xsl:include href="mol-tests.xsl"/><xsl:include href="plex-tests.xsl"/><xsl:include href="dimer-tests.xsl"/><xsl:include href="gpa-tests.xsl"/><xsl:include href="nuc-ex-tests.xsl"/><xsl:include href="kinase-tests.xsl"/><xsl:include href="scaffold-tests.xsl"/><xsl:key match="explicit-species/*" name="species-name-key" use="@name"/><xsl:key match="species-streams/*" name="species-stream-name-key" use="@name"/><xsl:key match="dump-streams/*" name="dump-stream-name-key" use="@name"/><xsl:template name="test-species-name"><xsl:param name="the-species-name"/><xsl:if test="not(key('species-name-key', $the-species-name))"><xsl:element name="p"><xsl:element name="font"><xsl:attribute name="color">red</xsl:attribute><xsl:text>Unknown species `</xsl:text><xsl:value-of select="$the-species-name"/><xsl:text>'.</xsl:text></xsl:element></xsl:element></xsl:if></xsl:template><xsl:template name="test-species-name-unique"><xsl:param name="the-species-name"/><xsl:if test="count(key('species-name-key', $the-species-name)) &gt; 1"><xsl:element name="p"><xsl:element name="font"><xsl:attribute name="color">red</xsl:attribute><xsl:text>Duplicate species name `</xsl:text><xsl:value-of select="$the-species-name"/><xsl:text>'.</xsl:text></xsl:element></xsl:element></xsl:if></xsl:template><xsl:template name="test-species-stream-name"><xsl:param name="the-species-stream-name"/><xsl:if test="not(key('species-stream-name-key', $the-species-stream-name))"><xsl:element name="font"><xsl:attribute name="color">red</xsl:attribute><xsl:text>Unknown species-stream `</xsl:text><xsl:value-of select="$the-species-stream-name"/><xsl:text>'.</xsl:text></xsl:element></xsl:if></xsl:template><xsl:template name="test-species-stream-name-unique"><xsl:param name="the-species-stream-name"/><xsl:if test="count(key('species-stream-name-key', $the-species-stream-name)) &gt; 1"><xsl:element name="font"><xsl:attribute name="color">red</xsl:attribute><xsl:text>Duplicate species-stream name `</xsl:text><xsl:value-of select="$the-species-stream-name"/><xsl:text>'.</xsl:text></xsl:element></xsl:if></xsl:template><xsl:template name="test-dump-stream-name"><xsl:param name="the-dump-stream-name"/><xsl:if test="not(key('dump-stream-name-key', $the-dump-stream-name))"><xsl:element name="font"><xsl:attribute name="color">red</xsl:attribute><xsl:text>Unknown dump-stream `</xsl:text><xsl:value-of select="$the-dump-stream-name"/><xsl:text>'.</xsl:text></xsl:element></xsl:if></xsl:template><xsl:template name="test-dump-stream-name-unique"><xsl:param name="the-dump-stream-name"/><xsl:if test="count(key('dump-stream-name-key', $the-dump-stream-name)) &gt; 1"><xsl:element name="font"><xsl:attribute name="color">red</xsl:attribute><xsl:text>Duplicate dump-stream name `</xsl:text><xsl:value-of select="$the-dump-stream-name"/><xsl:text>'.</xsl:text></xsl:element></xsl:if></xsl:template><xsl:template match="*" mode="consistency"><!--This template is empty so that an element doesn't recursively display all the error messages from the elements lower in the heirarchy.--></xsl:template><xsl:template match="substrate-species-ref" mode="consistency"><xsl:call-template name="test-species-name"><xsl:with-param name="the-species-name" select="@name"/></xsl:call-template></xsl:template><xsl:template match="product-species-ref" mode="consistency"><xsl:call-template name="test-species-name"><xsl:with-param name="the-species-name" select="@name"/></xsl:call-template></xsl:template></xsl:stylesheet>