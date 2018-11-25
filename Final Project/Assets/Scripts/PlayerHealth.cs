using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class PlayerHealth : MonoBehaviour {

    private Image healthBar;
    private float maxHealth = 100f;
    public static float health;
    public GameObject endLevel;
    // Use this for initialization
    void Start () {
        healthBar = GetComponent<Image>();
        health = maxHealth;
	}
	
	// Update is called once per frame
	void Update () {
        healthBar.fillAmount = health / maxHealth;
        if (health <= 0) { //restart level
            //go to lose page
            endLevel.SetActive(true);
            //SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex);
        }
	}
}
