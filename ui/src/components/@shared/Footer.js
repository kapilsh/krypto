// Libs
import React, { Component } from "react";
import { Layout } from "antd";
// Styles
import styles from "./Footer.scss";
// Config
import { IMAGES } from "@/config";

/**
 * Footer
 *
 * @class Footer
 * @extends {Component}
 */
class Footer extends Component {
  constructor(props) {
    super(props);
  }

  render() {
    return (
      <Layout.Footer className={styles.footer}>
        <img alt="" className={styles.logo} src={IMAGES.LOGO} />
        Sharmaman Trading ©2019
      </Layout.Footer>
    );
  }
}

export default Footer;
