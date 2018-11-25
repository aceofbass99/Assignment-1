using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BasicEnemyTracking : MonoBehaviour {

    public float speed;
    private Transform target;
    private Rigidbody2D enemy;
    private Vector2 direction;
    public AudioSource source;
    public static AudioClip alien;
    private static Random rnd;
    private bool soundPlayed;

	// Use this for initialization
	void Start () {
        target = GameObject.FindGameObjectWithTag("Player").GetComponent<Transform>();
        enemy = GetComponent<Rigidbody2D>();
        source = GetComponent<AudioSource>();
	}
	
	// Update is called once per frame
	void Update () {
        direction = target.transform.position - transform.position;
        if (direction.magnitude <= 10)
        {
            if (!soundPlayed) {
                string[] sounds = { "sfx_deathscream_alien1", "sfx_deathscream_alien2", "sfx_deathscream_alien3", "sfx_deathscream_alien4", "sfx_deathscream_alien5" };
                int randomNum = Random.Range(1, 5);
                int soundIndex = Mathf.RoundToInt(randomNum);
                alien = Resources.Load<AudioClip>(sounds[soundIndex]);
                source.clip = alien;
                source.Play();
                soundPlayed = true;
            }
            
            direction.Normalize();
            direction *= 50;
            enemy.velocity = speed * Time.deltaTime * direction;
            float rotation = (Mathf.Atan2(direction.y, direction.x) * Mathf.Rad2Deg) - 180.0f;
            transform.rotation = Quaternion.Euler(0.0f, 0.0f, rotation);
        }
	}

    void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.tag == "Player")
        {
            Destroy(this.gameObject);
            PlayerHealth.health -= 10;
        }    
    }
}
